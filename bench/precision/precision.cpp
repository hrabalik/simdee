#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <random>
#include <simdee/simdee.hpp>

#if SIMDEE_NEON
#include <arm_neon.h>
#endif

// #define EXHAUSTIVE

const int g_retries = 10000000;

template <typename Routine>
void prec_test(const char* name, Routine routine) {
    std::minstd_rand rand(0x0f15aa32);
    std::array<uint32_t, 9> results = {};

#ifdef EXHAUSTIVE
    for (uint64_t i = 0; i <= uint32_t(-1); i++) {
#else
    for (int i = 0; i < g_retries; i++) {
#endif

        uint32_t ux = rand() & 0x7fffffff;
        float x;
        ::memcpy(&x, &ux, sizeof(float));
        if (std::isnan(x)) { continue; }
        if (std::isinf(x)) { continue; }
        float gt_r = ::sqrtf(x);
        float r = routine(x);
        int gt_r_i;
        int r_i;
        static_assert(sizeof(int) == sizeof(float), "");
        ::memcpy(&gt_r_i, &gt_r, sizeof(float));
        ::memcpy(&r_i, &r, sizeof(float));
        int error_ulps = r_i - gt_r_i;
        auto result_idx = uint32_t(4 + std::max(-4, std::min(4, error_ulps)));
        results[result_idx]++;
    }
    uint32_t valid_retries = std::accumulate(begin(results), end(results), 0u);
    std::printf("%s\n", name);
    std::printf("  total          %u\n", valid_retries);
    std::printf("  correct        %u\n", results[4]);
    std::printf("  completely off %u\n", results[0] + results[8]);
    std::printf("  3 ULPs below   %u\n", results[1]);
    std::printf("  2 ULPs below   %u\n", results[2]);
    std::printf("  1 ULP below    %u\n", results[3]);
    std::printf("  1 ULP above    %u\n", results[5]);
    std::printf("  2 ULPs above   %u\n", results[6]);
    std::printf("  3 ULPs above   %u\n", results[7]);
}

int main() {
#if SIMDEE_NEON
    prec_test("NEON approximate raw", [](float x) {
        float32x4_t vx = vmovq_n_f32(x);
        float32x4_t r = vrsqrteq_f32(vx);

        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));

        r = vmulq_f32(vx, r);
        return vgetq_lane_f32(r, 0);
    });
    prec_test("NEON approximate +1", [](float x) {
        float32x4_t vx = vmovq_n_f32(x);
        float32x4_t r = vrsqrteq_f32(vx);

        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));

        r = vmulq_f32(vx, r);

        uint32x4_t one = vmovq_n_u32(1u);
        float32x4_t rp1 = vreinterpretq_f32_u32(vaddq_u32(vreinterpretq_u32_f32(r), one));
        r = vbslq_f32(vcltq_f32(vmulq_f32(r, r), vx), rp1, r);

        return vgetq_lane_f32(r, 0);
    });
#endif
}
