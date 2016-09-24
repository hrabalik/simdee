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
    enum class Result : char { fail = 13, win = 42 };
    std::vector<Result> resultsNonSimd(8 * numData);
    std::vector<Result> resultsHandSimd(8 * numData);
    std::vector<Result> resultsLibSimd(8 * numData);

    // fill data
    std::random_device rd;
    std::minstd_rand re(0x8a7ac012);
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
    struct Vec3f { float x, y, z; };
    const float robustFactor = 1 + 2 * gamma(3);
    const Vec3f invDir = { dist(re), dist(re), dist(re) };
    const Vec3f rayOrigin = { dist(re), dist(re), dist(re) };
    const float rayTMax = 100.f;
    const int dirIsNeg[3] = { (dist(re) > 0) ? 1 : 0, (dist(re) > 0) ? 1 : 0, (dist(re) > 0) ? 1 : 0 };

    // non-SIMD implementation (for correctness checking)
    auto nonSimd = [&]() {
        auto resIt = resultsNonSimd.begin();

        for (const auto& elem : data) {
            for (int i = 0; i < 8; ++i) {
                Vec3f bounds[2] = {
                    { elem.minx[i], elem.miny[i], elem.minz[i] },
                    { elem.maxx[i], elem.maxy[i], elem.maxz[i] },
                };
                float tmin = (bounds[dirIsNeg[0]].x - rayOrigin.x) * invDir.x;
                float tmax = (bounds[1 - dirIsNeg[0]].x - rayOrigin.x) * invDir.x;
                float tminy = (bounds[dirIsNeg[1]].y - rayOrigin.y) * invDir.y;
                float tmaxy = (bounds[1 - dirIsNeg[1]].y - rayOrigin.y) * invDir.y;
                tmax *= robustFactor;
                tmaxy *= robustFactor;
                if (tmin > tmaxy || tminy > tmax) {
                    *(resIt++) = Result::fail;
                    continue;
                }
                if (tminy > tmin) tmin = tminy;
                if (tmaxy < tmax) tmax = tmaxy;
                float tminz = (bounds[dirIsNeg[2]].z - rayOrigin.z) * invDir.z;
                float tmaxz = (bounds[1 - dirIsNeg[2]].z - rayOrigin.z) * invDir.z;
                tmaxz *= robustFactor;
                if (tmin > tmaxz || tminz > tmax) {
                    *(resIt++) = Result::fail;
                    continue;
                }
                if (tminz > tmin) tmin = tminz;
                if (tmaxz < tmax) tmax = tmaxz;
                *(resIt++) = ((tmin < rayTMax) && (tmax > 0)) ? Result::win : Result::fail;
            }
        }
    };

    // SIMD implementation by hand
    auto handSimd = [&]() {
        auto resIt = resultsHandSimd.begin();

        for (const auto& elem : data) {
            uint32_t fail;
            __m256 tmin, tmax;
            {
                __m256 minx = _mm256_load_ps(elem.minx);
                __m256 maxx = _mm256_load_ps(elem.maxx);
                __m256 idirx = _mm256_broadcast_ss(&invDir.x);
                __m256 rayox = _mm256_broadcast_ss(&rayOrigin.x);
                tmin = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[0] ? maxx : minx, rayox), idirx);
                tmax = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[0] ? minx : maxx, rayox), idirx);
            }

            {
                __m256 tminy, tmaxy;
                {
                    __m256 miny = _mm256_load_ps(elem.miny);
                    __m256 maxy = _mm256_load_ps(elem.maxy);
                    __m256 idiry = _mm256_broadcast_ss(&invDir.y);
                    __m256 rayoy = _mm256_broadcast_ss(&rayOrigin.y);
                    tminy = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[1] ? maxy : miny, rayoy), idiry);
                    tmaxy = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[1] ? miny : maxy, rayoy), idiry);
                }

                __m256 factor = _mm256_broadcast_ss(&robustFactor);
                tmax = _mm256_mul_ps(tmax, factor);
                tmaxy = _mm256_mul_ps(tmaxy, factor);

                {
                    __m256 failcond = _mm256_or_ps(_mm256_cmp_ps(tmin, tmaxy, _CMP_GT_OQ), _mm256_cmp_ps(tminy, tmax, _CMP_GT_OQ));
                    fail = uint32_t(_mm256_movemask_ps(failcond));

                    if (fail == 0xFF) {
                        for (int i = 0; i < 8; ++i) {
                            *(resIt++) = Result::fail;
                        }
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
                    __m256 idirz = _mm256_broadcast_ss(&invDir.z);
                    __m256 rayoz = _mm256_broadcast_ss(&rayOrigin.z);
                    tminz = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[2] ? maxz : minz, rayoz), idirz);
                    tmaxz = _mm256_mul_ps(_mm256_sub_ps(dirIsNeg[2] ? minz : maxz, rayoz), idirz);
                }

                __m256 factor = _mm256_broadcast_ss(&robustFactor);
                tmaxz = _mm256_mul_ps(tmaxz, factor);

                {
                    __m256 failcond = _mm256_or_ps(_mm256_cmp_ps(tmin, tmaxz, _CMP_GT_OQ), _mm256_cmp_ps(tminz, tmax, _CMP_GT_OQ));
                    fail = fail | uint32_t(_mm256_movemask_ps(failcond));

                    if (fail == 0xFF) {
                        for (int i = 0; i < 8; ++i) {
                            *(resIt++) = Result::fail;
                        }
                        continue;
                    }
                }

                tmin = _mm256_blendv_ps(tmin, tminz, _mm256_cmp_ps(tminz, tmin, _CMP_GT_OQ));
                tmax = _mm256_blendv_ps(tmax, tmaxz, _mm256_cmp_ps(tmaxz, tmax, _CMP_LT_OQ));
            }

            __m256 wincond = _mm256_and_ps(_mm256_cmp_ps(tmin, _mm256_broadcast_ss(&rayTMax), _CMP_LT_OQ), _mm256_cmp_ps(tmax, _mm256_setzero_ps(), _CMP_GT_OQ));
            uint32_t win = ~fail & uint32_t(_mm256_movemask_ps(wincond));

            for (int i = 0; i < 8; ++i) {
                *(resIt++) = ((win >> i) & 1) ? Result::win : Result::fail;
            }
        }
    };

    nonSimd();
    handSimd();
    REQUIRE(resultsNonSimd == resultsHandSimd);
}
