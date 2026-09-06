#include "bumped_vol_surface.hpp"

BumpedVolSurface::BumpedVolSurface(const VolSurface& base,
                                   double spot_multiplier,
                                   double ssr)
    : base_(base), spot_multiplier_(spot_multiplier), ssr_(ssr) {}

double BumpedVolSurface::forward(double t) const {
    // Spot is multiplied by spot_multiplier_; rates and dividends are unchanged,
    // so the whole forward curve scales by that factor.
    //
    // TODO: return the bumped forward.
    (void)t;
    return 0.0;
}

double BumpedVolSurface::impliedVol(double strike, double t) const {
    // Query the *base* surface at an SSR-adjusted strike so that:
    //
    //  * ssr = 0 (sticky delta): vol'(m * F'(t), t) = vol(m * F(t), t)
    //    for all moneyness m > 0. In particular ATM vol is unchanged.
    //  * ssr = 1 (sticky strike): vol'(K, t) = vol(K, t) for all K.
    //  * general ssr: the smile in log-moneyness k = ln(K/F) slides by
    //    ssr * dlnS. Equivalently, the ATM finite-difference identity
    //      Δσ_ATM ≈ ssr * (∂σ/∂k)|_{k=0} * dlnS
    //    holds to first order (this is Bergomi's definition of SSR).
    //
    // TODO: return the bumped implied vol.
    (void)strike;
    (void)t;
    return 0.0;
}
