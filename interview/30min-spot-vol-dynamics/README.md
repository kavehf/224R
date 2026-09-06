# 30 min — spot vol dynamics (skew stickiness ratio)

Pair-coding. Implement a **bumped** implied-vol surface on top of an opaque
`VolSurface`. The concrete parametrisation (SVI with a term structure) is
deliberately hidden; you only get `forward(t)` and `impliedVol(K, t)`.

## What you implement

`src/bumped_vol_surface.cpp` — two methods on `BumpedVolSurface`:

1. `forward(t)` after a relative spot bump `S → S * spot_multiplier`, with
   rates and dividends unchanged.
2. `impliedVol(K, t)` under a constant **skew stickiness ratio** (SSR):
   - `ssr = 0` — **sticky delta** / sticky log-moneyness
   - `ssr = 1` — **sticky strike**
   - general `ssr` interpolates between them (equity index typically ~1.5–2)

The header `include/bumped_vol_surface.hpp` states the required properties.
Do not open `hidden/` or `solution/`.

## Build and run

```bash
cmake -S . -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/test_properties
./build/print_smile
```

Or `make` from this directory.

`test_properties` checks behaviour (forwards, sticky-delta, sticky-strike,
and the first-order ATM identity that defines SSR). It does not spell out
the strike map.

## Assumptions you can take as given

- Expiries `t` are in years, strikes are absolute (not log-moneyness).
- The spot multiplier is strictly positive.
- SSR is constant across expiry (no `ssr(T)` in this exercise).
- “Sticky delta” here means sticky **log-moneyness** `k = ln(K/F)`, which is
  the usual first-order proxy for sticky Black delta.
