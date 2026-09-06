#include "bumped_vol_surface.hpp"
#include "surface_factory.hpp"
#include "test_harness.hpp"

#include <cmath>
#include <vector>

// INTERVIEWER ONLY. This asserts the closed-form strike map
//   K_old = K * u^{ssr - 1}
// and would give the implementation away if shown to the candidate.

int main() {
    const auto base = make_sample_surface();
    const double u = 1.025;
    const std::vector<double> ssrs = {0.0, 0.5, 1.0, 1.5, 2.0};
    const std::vector<double> expiries = {0.25, 1.0, 2.0};
    const std::vector<double> strikes = {85.0, 100.0, 115.0};

    for (double ssr : ssrs) {
        const BumpedVolSurface bumped(*base, u, ssr);
        for (double t : expiries) {
            for (double K : strikes) {
                const double K_old = K * std::pow(u, ssr - 1.0);
                EXPECT_NEAR(bumped.impliedVol(K, t), base->impliedVol(K_old, t), 1e-10);
            }
        }
    }
    return test_report();
}
