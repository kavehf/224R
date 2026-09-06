#pragma once

#include "vol_surface.hpp"

#include <vector>

// INTERVIEWER ONLY. Do not open this during the candidate session.
//
// Raw SVI total variance on a slice:
//   w(k) = a + b * (rho * (k - m) + sqrt((k - m)^2 + sigma^2))
// with k = ln(K / F(t)).
//
// Term structure: linear interpolation of total variance at fixed k between
// neighbouring slices. Outside the first/last expiry, implied vol is frozen
// (total variance scales with t).
struct SviSlice {
    double t{};
    double a{};
    double b{};
    double rho{};
    double m{};
    double sigma{};
};

class SviVolSurface : public VolSurface {
public:
    SviVolSurface(double spot,
                  double rate,
                  double dividend,
                  std::vector<SviSlice> slices);

    double forward(double t) const override;
    double impliedVol(double strike, double t) const override;

private:
    double totalVariance(double k, double t) const;
    static double sliceTotalVariance(const SviSlice& slice, double k);

    double spot_;
    double rate_;
    double dividend_;
    std::vector<SviSlice> slices_;  // sorted by t, strictly increasing
};
