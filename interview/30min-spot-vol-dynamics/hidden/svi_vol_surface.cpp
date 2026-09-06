#include "svi_vol_surface.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

SviVolSurface::SviVolSurface(double spot,
                             double rate,
                             double dividend,
                             std::vector<SviSlice> slices)
    : spot_(spot), rate_(rate), dividend_(dividend), slices_(std::move(slices)) {
    if (spot_ <= 0.0) {
        throw std::invalid_argument("spot must be positive");
    }
    if (slices_.size() < 1) {
        throw std::invalid_argument("need at least one SVI slice");
    }
    for (std::size_t i = 0; i < slices_.size(); ++i) {
        if (slices_[i].t <= 0.0) {
            throw std::invalid_argument("slice expiry must be positive");
        }
        if (i > 0 && slices_[i].t <= slices_[i - 1].t) {
            throw std::invalid_argument("SVI slices must be strictly increasing in t");
        }
    }
}

double SviVolSurface::forward(double t) const {
    return spot_ * std::exp((rate_ - dividend_) * t);
}

double SviVolSurface::impliedVol(double strike, double t) const {
    if (strike <= 0.0 || t <= 0.0) {
        throw std::invalid_argument("strike and expiry must be positive");
    }
    const double F = forward(t);
    const double k = std::log(strike / F);
    const double w = totalVariance(k, t);
    return std::sqrt(std::max(w, 0.0) / t);
}

double SviVolSurface::sliceTotalVariance(const SviSlice& slice, double k) {
    const double x = k - slice.m;
    return slice.a + slice.b * (slice.rho * x + std::sqrt(x * x + slice.sigma * slice.sigma));
}

double SviVolSurface::totalVariance(double k, double t) const {
    const auto& first = slices_.front();
    const auto& last = slices_.back();

    if (t <= first.t) {
        return sliceTotalVariance(first, k) * (t / first.t);
    }
    if (t >= last.t) {
        return sliceTotalVariance(last, k) * (t / last.t);
    }

    const auto it = std::upper_bound(
        slices_.begin(), slices_.end(), t,
        [](double expiry, const SviSlice& slice) { return expiry < slice.t; });
    const SviSlice& hi = *it;
    const SviSlice& lo = *(it - 1);
    const double w_lo = sliceTotalVariance(lo, k);
    const double w_hi = sliceTotalVariance(hi, k);
    const double a = (t - lo.t) / (hi.t - lo.t);
    return (1.0 - a) * w_lo + a * w_hi;
}
