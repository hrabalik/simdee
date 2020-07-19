#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <simdee/simdee.hpp>

template <typename T>
T sqr(T x) {
    return x * x;
}

double normal_cdf(double x, double mean, double var) {
    if (var == 0.) { return (x < mean) ? 0. : 1.; }
    return 0.5 + 0.5 * std::erf((x - mean) / std::sqrt(2. * var));
}

class stats {
private:
    int64_t m_sum_x0 = 0;
    int64_t m_sum_x1 = 0;
    int64_t m_sum_x2 = 0;

public:
    void update(int64_t x) {
        m_sum_x0 += 1;
        m_sum_x1 += x;
        m_sum_x2 += sqr(x);
    }

    double get_mean() const { return double(m_sum_x1) / double(m_sum_x0); }

    double get_var() const {
        double e_x1_sqr = double(sqr(m_sum_x1)) / double(sqr(m_sum_x0));
        double e_x2 = double(m_sum_x2) / double(m_sum_x0);
        return std::max(0., e_x2 - e_x1_sqr);
    }
};

double confidence_gt0(const stats& s) {
    double mean = s.get_mean();
    double var = s.get_var();
    return normal_cdf(0., -mean, var);
}

#if SIMDEE_SSE2

using simd_vector_t = __m128;

__m128 initializer() {
    __m128 r = _mm_setr_ps(11.1e11f, 22.2e12f, 33.3e13f, 44.4e14f);
    return r;
}

void finalizer(const __m128&) {
    float f = 0.f;
    f += _mm_cvtss_f32(x);
    f += _mm_cvtss_f32(_mm_shuffle_ps(x, x, _MM_SHUFFLE(1, 1, 1, 1)));
    f += _mm_cvtss_f32(_mm_shuffle_ps(x, x, _MM_SHUFFLE(2, 2, 2, 2)));
    f += _mm_cvtss_f32(_mm_shuffle_ps(x, x, _MM_SHUFFLE(3, 3, 3, 3)));
    std::printf("Your lucky number is: %f\n", double(f));
}

SIMDEE_INL __m128 fun0(const __m128& x) { return x; }

SIMDEE_INL __m128 fun1(const __m128& x) { return x; }

#endif // SIMDEE_SSE2

#if SIMDEE_NEON

using simd_vector_t = float32x4_t;

float32x4_t initializer() {
    float32x4_t r = {11e11f, 12e12f, 13e13f, 14e14f};
    return r;
}

void finalizer(const float32x4_t& x) {
    float f = 0.f;
    f += vgetq_lane_f32(x, 0);
    f += vgetq_lane_f32(x, 1);
    f += vgetq_lane_f32(x, 2);
    f += vgetq_lane_f32(x, 3);
    std::printf("Your lucky number is: %f\n", double(f));
}

SIMDEE_INL float32x4_t fun0(const float32x4_t& x) {
    float32x4_t r = vmovq_n_f32(0);
    r = vsetq_lane_f32(std::sqrt(vgetq_lane_f32(x, 0)), r, 0);
    r = vsetq_lane_f32(std::sqrt(vgetq_lane_f32(x, 1)), r, 1);
    r = vsetq_lane_f32(std::sqrt(vgetq_lane_f32(x, 2)), r, 2);
    r = vsetq_lane_f32(std::sqrt(vgetq_lane_f32(x, 3)), r, 3);
    return r;
}

SIMDEE_INL float32x4_t fun1(const float32x4_t& x) {
    float l0 = std::sqrt(vgetq_lane_f32(x, 0));
    float l1 = std::sqrt(vgetq_lane_f32(x, 1));
    float32x2_t l = {l0, l1};
    float l2 = std::sqrt(vgetq_lane_f32(x, 2));
    float l3 = std::sqrt(vgetq_lane_f32(x, 3));
    float32x2_t h = {l2, l3};
    float32x4_t r = vcombine_f32(l, h);
    return r;
}

#endif // SIMDEE_NEON

const int batch_size = 100'000;
const int batch_count_min = 100;
const int batch_count_max = 1'000;

using clk = std::chrono::steady_clock;

template <typename Func>
clk::duration time(Func func) {
    clk::time_point started_tp = clk::now();
    func();
    clk::time_point finished_tp = clk::now();
    return finished_tp - started_tp;
}

int main() {
    stats stats0;
    stats stats1;
    stats stats_diff;
    simd_vector_t unopt_vec0 = initializer();
    simd_vector_t unopt_vec1 = initializer();
    int j = 0;
    for (; j < batch_count_max; j++) {
        if (j >= batch_count_min) {
            double confidence = confidence_gt0(stats_diff);
            if (std::min(confidence, 1. - confidence) < 5e-6) { break; }
        }
        clk::duration duration0 = time([&unopt_vec0]() {
            simd_vector_t x = unopt_vec0;
            for (int i = 0; i < batch_size; i++) { x = fun0(x); }
            unopt_vec0 = x;
        });
        clk::duration duration1 = time([&unopt_vec1]() {
            simd_vector_t x = unopt_vec1;
            for (int i = 0; i < batch_size; i++) { x = fun1(x); }
            unopt_vec1 = x;
        });
        stats0.update(duration0.count());
        stats1.update(duration1.count());
        stats_diff.update(duration1.count() - duration0.count());
    }
    std::printf("Stopping after %d iterations\n", j);
    finalizer(unopt_vec0);
    finalizer(unopt_vec1);
    std::printf("Duration 0: %.0f\n", stats0.get_mean());
    std::printf("Duration 1: %.0f\n", stats1.get_mean());
    double confidence = confidence_gt0(stats_diff);
    std::printf("Confidence 0 faster: %.3f %%\n", 100. * confidence);
    std::printf("Confidence 1 faster: %.3f %%\n", 100. * (1. - confidence));
}
