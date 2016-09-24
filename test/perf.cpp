#include "catch.hpp"
#include <limits>
#include <vector>
#include <random>
#include <simdify/simd_types.hpp>
#include <simdify/util/malloc.hpp>

TEST_CASE("Ray-box intersection", "[perf]") {
    // allocate data
    struct RayBoxData {
        float minx[8];
        float miny[8];
        float minz[8];
        float maxx[8];
        float maxy[8];
        float maxz[8];
    };
    const size_t numData = 1024;
    using vec_t = std::vector<RayBoxData, simd::aligned_allocator<RayBoxData, sizeof(__m256)>>;
    vec_t data(numData);
    std::vector<char> results(8 * numData);

    // fill data
    std::random_device rd;
    std::minstd_rand re(rd());
    std::normal_distribution<float> dist(0, 1);
    auto fill = [&dist, &re](float* data, size_t num) {
        for (size_t i = 0; i < num; ++i) {
            *(data++) = dist(re);
        }
    };
    const size_t numFloats = (sizeof(RayBoxData) / sizeof(float)) * numData;
    fill((float*)(data.data()), numFloats);

    // common pre-calculations
    auto gamma = [](int n) {
        float eps2 = static_cast<float>(std::numeric_limits<float>::epsilon() * 0.5);
        return (n * eps2) / (1 - n * eps2);
    };
    const float robustFactor = 1 + 2 * gamma(3);
    const float invDir[3] = { dist(re), dist(re), dist(re) };
    const float rayOrigin[3] = { dist(re), dist(re), dist(re) };
    const float rayTMax = 100.f;
    const int dirIsNeg[3] = { dist(re) > 0, dist(re) > 0, dist(re) > 0 };

    // implementation by hand
    auto hand_impl = [&]() {
        auto resIt = results.begin();

        for (const auto& elem : data) {
            __m256 tmin, tmax;
            {
                __m256 minx = _mm256_load_ps(elem.minx);
                __m256 maxx = _mm256_load_ps(elem.maxx);
                __m256 idirx = _mm256_broadcast_ss(&invDir[0]);
                __m256 rayox = _mm256_broadcast_ss(&rayOrigin[0]);
                tmin = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[0] ? maxx : minx, rayox), idirx);
                tmax = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[0] ? minx : maxx, rayox), idirx);
            }

            {
                __m256 tminy, tmaxy;
                {
                    __m256 miny = _mm256_load_ps(elem.miny);
                    __m256 maxy = _mm256_load_ps(elem.maxy);
                    __m256 idiry = _mm256_broadcast_ss(&invDir[1]);
                    __m256 rayoy = _mm256_broadcast_ss(&rayOrigin[1]);
                    tminy = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[1] ? maxy : miny, rayoy), idiry);
                    tmaxy = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[1] ? miny : maxy, rayoy), idiry);
                }

                __m256 factor = _mm256_broadcast_ss(&robustFactor);
                tmax = _mm256_mul_ps(tmax, factor);
                tmaxy = _mm256_mul_ps(tmaxy, factor);

                {
                    __m256 fail = _mm256_or_ps(_mm256_cmp_ps(tmin, tmaxy, _CMP_GT_OQ), _mm256_cmp_ps(tminy, tmax, _CMP_GT_OQ));
                    fail = _mm256_and_ps(fail, _mm256_permute_ps(fail, _MM_SHUFFLE(2, 3, 0, 1)));
                    fail = _mm256_and_ps(fail, _mm256_permute_ps(fail, _MM_SHUFFLE(1, 0, 3, 2)));
                    fail = _mm256_and_ps(fail, _mm256_permute2f128_ps(fail, fail, _MM_SHUFFLE(0, 0, 0, 1)));
                    float all_failed = _mm_cvtss_f32(_mm256_castps256_ps128(fail));

                    if (simd::tou(all_failed)) {
                        // mark fail
                        continue;
                    }
                }

                tmin = _mm256_blendv_ps(tmin, tminy, _mm256_cmp_ps(tminy, tmin, _CMP_GT_OQ));
                tmax = _mm256_blendv_ps(tmax, tmaxy, _mm256_cmp_ps(tmaxy, tmax, _CMP_LT_OQ));
            }

            {
                __m256 tminz, tmaxz;
                {
                    __m256 minz = _mm256_load_ps(elem.minz);
                    __m256 maxz = _mm256_load_ps(elem.maxz);
                    __m256 idirz = _mm256_broadcast_ss(&invDir[2]);
                    __m256 rayoz = _mm256_broadcast_ss(&rayOrigin[2]);
                    tminz = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[2] ? maxz : minz, rayoz), idirz);
                    tmaxz = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[2] ? minz : maxz, rayoz), idirz);
                }

                __m256 factor = _mm256_broadcast_ss(&robustFactor);
                tmaxz = _mm256_mul_ps(tmaxz, factor);

                {
                    __m256 fail = _mm256_or_ps(_mm256_cmp_ps(tmin, tmaxz, _CMP_GT_OQ), _mm256_cmp_ps(tminz, tmax, _CMP_GT_OQ));
                    fail = _mm256_and_ps(fail, _mm256_permute_ps(fail, _MM_SHUFFLE(2, 3, 0, 1)));
                    fail = _mm256_and_ps(fail, _mm256_permute_ps(fail, _MM_SHUFFLE(1, 0, 3, 2)));
                    fail = _mm256_and_ps(fail, _mm256_permute2f128_ps(fail, fail, _MM_SHUFFLE(0, 0, 0, 1)));
                    float all_failed = _mm_cvtss_f32(_mm256_castps256_ps128(fail));

                    if (simd::tou(all_failed)) {
                        // mark fail
                        continue;
                    }
                }

                tmin = _mm256_blendv_ps(tmin, tminz, _mm256_cmp_ps(tminz, tmin, _CMP_GT_OQ));
                tmax = _mm256_blendv_ps(tmax, tmaxz, _mm256_cmp_ps(tmaxz, tmax, _CMP_LT_OQ));
            }

            __m256 win = _mm256_and_ps(_mm256_cmp_ps(tmin, _mm256_broadcast_ss(&rayTMax), _CMP_LT_OQ), _mm256_cmp_ps(tmax, _mm256_setzero_ps(), _CMP_GT_OQ));
            
            uint32_t uwin[8];
            std::memcpy(uwin, &win, sizeof(__m256));
            for (int i = 0; i < 8; ++i) {
                *(resIt++) = uwin[i] ? 1 : 0;
            }
        }
    };
}
