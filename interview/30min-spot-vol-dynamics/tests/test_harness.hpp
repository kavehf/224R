#pragma once

#include <cmath>
#include <iostream>
#include <string>

struct TestStats {
    int passed = 0;
    int failed = 0;
    int printed_failures = 0;
};

inline TestStats& test_stats() {
    static TestStats stats;
    return stats;
}

inline void expect_near(const char* file,
                        int line,
                        const char* expr,
                        double got,
                        double expected,
                        double tol) {
    if (std::isfinite(got) && std::isfinite(expected) && std::fabs(got - expected) <= tol) {
        ++test_stats().passed;
        return;
    }
    ++test_stats().failed;
    constexpr int kMaxPrinted = 8;
    if (test_stats().printed_failures < kMaxPrinted) {
        ++test_stats().printed_failures;
        std::cout << "  " << file << ":" << line << "  FAIL  " << expr
                  << "  got=" << got << "  expected=" << expected << "  tol=" << tol << "\n";
    } else if (test_stats().printed_failures == kMaxPrinted) {
        ++test_stats().printed_failures;
        std::cout << "  ... further assertion dumps suppressed ...\n";
    }
}

#define EXPECT_NEAR(got, expected, tol) \
    expect_near(__FILE__, __LINE__, #got " ~ " #expected, (got), (expected), (tol))

struct TestSuite {
    const char* name;
    int failed_before;
    explicit TestSuite(const char* n) : name(n), failed_before(test_stats().failed) {
        std::cout << name << " ... " << std::flush;
    }
    ~TestSuite() {
        const int df = test_stats().failed - failed_before;
        if (df == 0) {
            std::cout << "ok\n";
        } else {
            std::cout << "FAILED (" << df << " assertions)\n";
        }
    }
};

inline int test_report() {
    std::cout << test_stats().passed << " passed, " << test_stats().failed << " failed\n";
    return test_stats().failed == 0 ? 0 : 1;
}
