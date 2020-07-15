#pragma once
#include "stats_printer.hpp"
#include <cstring>
#include <random>

const int g_non_exhaustive_retries = 10000000;

struct run_opts {
    bool exhaustive;
};

template <typename GtRoutine, typename Routine>
void run(const run_opts& opts, const char* name, bool positive_only, GtRoutine gt_routine,
         Routine routine) {
    stats_printer sp(name);
    auto process = [gt_routine, routine, &sp](uint32_t ux) {
        float x;
        std::memcpy(&x, &ux, sizeof(float));
        if (std::isnan(x) || std::isinf(x)) { return; }
        float value = routine(x);
        float gt = gt_routine(x);
        sp.update(value, gt);
    };
    uint32_t mask = positive_only ? 0x7fffffff : 0xffffffff;
    if (opts.exhaustive) {
        for (uint32_t i = 0; i < mask; i++) { process(i); }
    } else {
        std::minstd_rand rand(0x0f15aa32);
        for (int i = 0; i < g_non_exhaustive_retries; i++) { process(uint32_t(rand()) & mask); }
    }
}
