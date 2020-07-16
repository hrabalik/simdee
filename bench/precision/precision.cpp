#include "run.hpp"
#include <simdee/simdee.hpp>

float gt_recp(float x) { return 1 / x; }
float gt_rsqrt(float x) { return float(1 / std::sqrt(double(x))); }
float gt_sqrt(float x) { return std::sqrt(x); }

int main(int argc, char** argv) {
    run_opts opts = {};
    struct {
        bool recp, rsqrt, sqrt;
    } algos = {};
    bool abort = argc < 2;

    for (int i = 1; i < argc; i++) {
        if (0 == std::strcmp(argv[i], "--exhaustive")) {
            opts.exhaustive = true;
        } else if (0 == std::strcmp(argv[i], "--recp")) {
            algos.recp = true;
        } else if (0 == std::strcmp(argv[i], "--rsqrt")) {
            algos.rsqrt = true;
        } else if (0 == std::strcmp(argv[i], "--sqrt")) {
            algos.sqrt = true;
        } else if (0 == std::strcmp(argv[i], "--all")) {
            algos.recp = true;
            algos.rsqrt = true;
            algos.sqrt = true;
        } else {
            abort = true;
            break;
        }
    }

    if (abort) {
        std::printf(
            "Usage: simdee-precision [--exhaustive] [--recp] [--rsqrt] [--sqrt]\n"
            "Options:\n"
            "  --exhaustive\n"
            "    Performs the operation with all possible inputs, as opposed to a number of\n"
            "    randomly-selected inputs.\n"
            "  --recp / --rsqrt / --sqrt\n"
            "    Include certain tests (reciprocal, reciprocal square root, square root).\n"
            "  --all\n"
            "    Include all tests.\n");
        return -1;
    }

#if SIMDEE_SSE2
    if (algos.recp) {
        run(opts, "recp SSE2", false, gt_recp, [](float x) {
            __m128 vx = _mm_set_ps1(x);
            __m128 r = _mm_rcp_ps(vx);
            return _mm_cvtss_f32(r);
        });
    }
    if (algos.rsqrt) {
        run(opts, "rsqrt SSE2", true, gt_rsqrt, [](float x) {
            __m128 vx = _mm_set_ps1(x);
            __m128 r = _mm_rsqrt_ps(vx);
            return _mm_cvtss_f32(r);
        });
    }
#endif
#if SIMDEE_NEON
    if (algos.recp) {
        run(opts, "recp NEON iteration 0", false, gt_recp, [](float x) {
            float32x4_t vx = vmovq_n_f32(x);
            float32x4_t r = vrecpeq_f32(vx);
            return vgetq_lane_f32(r, 0);
        });
        run(opts, "recp NEON iteration 1", false, gt_recp, [](float x) {
            float32x4_t vx = vmovq_n_f32(x);
            float32x4_t r = vrecpeq_f32(vx);
            r = vmulq_f32(r, vrecpsq_f32(vx, r));
            return vgetq_lane_f32(r, 0);
        });
    }
    if (algos.rsqrt) {
        run(opts, "rsqrt NEON iteration 0", true, gt_rsqrt, [](float x) {
            float32x4_t vx = vmovq_n_f32(x);
            float32x4_t r = vrsqrteq_f32(vx);
            return vgetq_lane_f32(r, 0);
        });
        run(opts, "rsqrt NEON iteration 1", true, gt_rsqrt, [](float x) {
            float32x4_t vx = vmovq_n_f32(x);
            float32x4_t r = vrsqrteq_f32(vx);
            r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));
            return vgetq_lane_f32(r, 0);
        });
    }
    if (algos.sqrt) {
        run(opts, "sqrt NEON approx", true, gt_sqrt, [](float x) {
            float32x4_t vx = vmovq_n_f32(x);
            float32x4_t r = vrsqrteq_f32(vx);

            r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));
            r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));
            r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(vx, r), r));

            r = vmulq_f32(vx, r);
            return vgetq_lane_f32(r, 0);
        });
        run(opts, "sqrt NEON approx +1", true, gt_sqrt, [](float x) {
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
    }
#endif
}
