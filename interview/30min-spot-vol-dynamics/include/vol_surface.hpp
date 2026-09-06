#pragma once

// Market Black implied-vol surface in (strike, expiry) space.
//
// The concrete parametrisation (SVI with a term structure, ...) is hidden.
// Callers should depend only on this interface: a forward curve and an
// implied-vol function.
class VolSurface {
public:
    virtual ~VolSurface() = default;

    // Forward for expiry t, in years from today. Must be strictly positive.
    virtual double forward(double t) const = 0;

    // Black implied volatility for strike K > 0 and expiry t > 0, in years.
    virtual double impliedVol(double strike, double t) const = 0;
};
