#pragma once

#include "vol_surface.hpp"

// Vol surface after a relative spot bump S -> S * spot_multiplier, with
// rates and dividends held fixed (so every forward scales by the same factor).
//
// The smile is moved according to a constant skew-stickiness ratio (SSR):
//
//   SSR = 0  sticky delta / sticky log-moneyness
//            implied vol as a function of K/F is invariant; ATM vol does not
//            jump with spot.
//   SSR = 1  sticky strike
//            implied vol as a function of K is invariant; ATM vol rides the
//            existing skew.
//   SSR ~ 1.5–2  typical equity-index (Bergomi); ATM vol moves *more* than
//            sticky-strike would predict.
//
// The base surface must outlive this wrapper.
class BumpedVolSurface : public VolSurface {
public:
    BumpedVolSurface(const VolSurface& base,
                     double spot_multiplier,
                     double ssr);

    double forward(double t) const override;
    double impliedVol(double strike, double t) const override;

private:
    const VolSurface& base_;
    double spot_multiplier_;
    double ssr_;
};
