#pragma once

#include <algorithm>
#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <string>

class stats_printer {
private:
    std::string m_name;
    double m_max_rel_err = 0.;
    uint64_t m_retry_count = 0;
    uint64_t m_correct_count = 0;

public:
    stats_printer(const char* name) : m_name(name) {}

    ~stats_printer() {
        double correct_percent = 100. * double(m_correct_count) / double(m_retry_count);
        int max_rel_err_exp;
        double max_rel_err_fract = ::frexp(m_max_rel_err, &max_rel_err_exp);
        std::printf("%s\n"
                    "  total          %" PRIu64 "\n"
                    "  correct        %" PRIu64 " (%.1f%%)\n"
                    "  max rel error  %f * 2^%d\n",
                    m_name.c_str(), m_retry_count, m_correct_count, correct_percent,
                    max_rel_err_fract, max_rel_err_exp);
    }

    void update(double value, double gt) {
        m_retry_count++;
        if (value == gt) { m_correct_count++; }
        if ((gt != 0) && !std::isinf(value)) {
            double rel_err = std::abs((value - gt) / gt);
            m_max_rel_err = std::max(m_max_rel_err, rel_err);
        }
    }
};
