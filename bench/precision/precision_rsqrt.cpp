#include <algorithm>
#include <cmath>
#include <cstdio>
#include <random>
#include <simdee/simdee.hpp>

// #define EXHAUSTIVE

const int g_retries = 10000000;

template <typename Routine>
void prec_test(const char* name, Routine routine) {
    double max_rel_err = 0.;
    uint32_t retry_count = 0u;
    uint32_t correct_count = 0u;

#ifdef EXHAUSTIVE
    for (uint32_t i = 0; i <= 0x7fffffff; i++) {
        uint32_t ux = i;
#else
    std::minstd_rand rand(0x0f15aa32);
    for (int i = 0; i < g_retries; i++) {
        uint32_t ux = rand() & 0x7fffffff;
#endif

        float x;
        ::memcpy(&x, &ux, sizeof(float));
        if (std::isnan(x)) { continue; }
        if (std::isinf(x)) { continue; }
        float gt_r = float(1 / std::sqrt(double(x)));
        float r = routine(x);
        retry_count++;
        if (r == gt_r) { correct_count++; }
        double gt_r_dbl = gt_r;
        if ((gt_r_dbl != 0) && !std::isinf(r)) {
            double r_dbl = r;
            double rel_err = std::abs((r_dbl - gt_r_dbl) / gt_r_dbl);
            max_rel_err = std::max(max_rel_err, rel_err);
        }
    }
    int max_rel_err_exp;
    double max_rel_err_fract = ::frexp(max_rel_err, &max_rel_err_exp);
    std::printf("%s\n", name);
    std::printf("  total          %u\n", retry_count);
    std::printf("  correct        %u\n", correct_count);
    std::printf("  max rel error  %f * 2^%d\n", max_rel_err_fract, max_rel_err_exp);
}

int main() {
    // prec_test("long double",
    //           [](float x) { return float(1 / std::sqrt(static_cast<long double>(x))); });

#if SIMDEE_SSE2
    prec_test("SSE2", [](float x) {
        __m128 vx = _mm_set_ps1(x);
        __m128 r = _mm_rsqrt_ps(vx);
        return _mm_cvtss_f32(r);
    });
#endif
#if SIMDEE_NEON
    prec_test("NEON iteration 0", [](float x) {
        float32x4_t vx = vmovq_n_f32(x);
        float32x4_t r = vrsqrteq_f32(vx);
        return vgetq_lane_f32(r, 0);
    });
    prec_test("NEON iteration 1", [](float x) {
        float32x4_t vx = vmovq_n_f32(x);
        float32x4_t r = vrsqrteq_f32(vx);
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));
        return vgetq_lane_f32(r, 0);
    });
#endif
}
