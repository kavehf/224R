#include "bumped_vol_surface.hpp"
#include "surface_factory.hpp"
#include "test_harness.hpp"

#include <cmath>
#include <vector>

namespace {

const std::vector<double> kExpiries = {0.25, 0.5, 1.0, 2.0};
const std::vector<double> kMoneyness = {0.80, 0.90, 1.00, 1.10, 1.25};

void test_forward_scales_with_spot() {
    const TestSuite suite("forward scales with spot");
    const auto base = make_sample_surface();
    const double u = 1.02;
    const BumpedVolSurface bumped(*base, u, /*ssr=*/0.0);
    for (double t : kExpiries) {
        EXPECT_NEAR(bumped.forward(t), u * base->forward(t), 1e-12);
    }
}

void test_sticky_delta_preserves_moneyness_smile() {
    const TestSuite suite("sticky delta preserves moneyness smile");
    const auto base = make_sample_surface();
    const double u = 1.03;
    const BumpedVolSurface bumped(*base, u, /*ssr=*/0.0);
    for (double t : kExpiries) {
        const double F = base->forward(t);
        const double Fp = bumped.forward(t);
        for (double m : kMoneyness) {
            const double v_old = base->impliedVol(m * F, t);
            const double v_new = bumped.impliedVol(m * Fp, t);
            EXPECT_NEAR(v_new, v_old, 1e-10);
        }
    }
}

void test_sticky_strike_preserves_strike_smile() {
    const TestSuite suite("sticky strike preserves strike smile");
    const auto base = make_sample_surface();
    const double u = 1.03;
    const BumpedVolSurface bumped(*base, u, /*ssr=*/1.0);
    const std::vector<double> strikes = {80.0, 90.0, 100.0, 110.0, 125.0};
    for (double t : kExpiries) {
        for (double K : strikes) {
            EXPECT_NEAR(bumped.impliedVol(K, t), base->impliedVol(K, t), 1e-10);
        }
    }
}

// Bergomi's definition: d σ_ATM / d ln F = SSR * (∂σ/∂k)|_{k=0}.
void test_ssr_atm_first_order() {
    const TestSuite suite("SSR ATM first-order identity");
    const auto base = make_sample_surface();
    const double h = 1e-4;  // small so convexity is negligible
    const double u = std::exp(h);
    const std::vector<double> ssrs = {0.0, 1.0, 1.5, 2.0};
    const double eps = 1e-4;

    for (double ssr : ssrs) {
        const BumpedVolSurface bumped(*base, u, ssr);
        for (double t : kExpiries) {
            const double F = base->forward(t);
            const double atm0 = base->impliedVol(F, t);
            const double atm1 = bumped.impliedVol(bumped.forward(t), t);
            const double d_atm = atm1 - atm0;

            const double vp = base->impliedVol(F * std::exp(eps), t);
            const double vm = base->impliedVol(F * std::exp(-eps), t);
            const double skew = (vp - vm) / (2.0 * eps);

            EXPECT_NEAR(d_atm, ssr * skew * h, 5e-7);
        }
    }
}

void test_unit_multiplier_is_identity() {
    const TestSuite suite("unit multiplier is identity");
    const auto base = make_sample_surface();
    const BumpedVolSurface bumped(*base, /*spot_multiplier=*/1.0, /*ssr=*/1.7);
    for (double t : kExpiries) {
        EXPECT_NEAR(bumped.forward(t), base->forward(t), 1e-12);
        for (double m : kMoneyness) {
            const double K = m * base->forward(t);
            EXPECT_NEAR(bumped.impliedVol(K, t), base->impliedVol(K, t), 1e-12);
        }
    }
}

}  // namespace

int main() {
    test_forward_scales_with_spot();
    test_sticky_delta_preserves_moneyness_smile();
    test_sticky_strike_preserves_strike_smile();
    test_ssr_atm_first_order();
    test_unit_multiplier_is_identity();
    return test_report();
}
