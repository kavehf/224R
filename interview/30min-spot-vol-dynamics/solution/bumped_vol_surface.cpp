#include "bumped_vol_surface.hpp"

#include <cmath>

BumpedVolSurface::BumpedVolSurface(const VolSurface& base,
                                   double spot_multiplier,
                                   double ssr)
    : base_(base), spot_multiplier_(spot_multiplier), ssr_(ssr) {}

double BumpedVolSurface::forward(double t) const {
    // Unchanged rates/dividends: F'(t) = u * F(t), u = S'/S.
    return spot_multiplier_ * base_.forward(t);
}

double BumpedVolSurface::impliedVol(double strike, double t) const {
    // Let u = S'/S, h = ln u, k' = ln(K / F').
    // Bergomi SSR moves the smile in log-moneyness by ssr * h:
    //
    //   σ'(k') = σ(k' + ssr * h)
    //
    // so the old-surface strike to query is
    //   K_old = F * exp(k' + ssr * h) = K * u^{ssr - 1}.
    //
    // Checks:
    //   ssr = 0 -> K_old = K / u = K * F / F'   (sticky moneyness)
    //   ssr = 1 -> K_old = K                    (sticky strike)
    const double h = std::log(spot_multiplier_);
    const double F_old = base_.forward(t);
    const double F_new = spot_multiplier_ * F_old;
    const double k_new = std::log(strike / F_new);
    const double K_old = F_old * std::exp(k_new + ssr_ * h);
    return base_.impliedVol(K_old, t);
}
