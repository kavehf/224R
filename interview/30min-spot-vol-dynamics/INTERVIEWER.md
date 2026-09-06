# Interviewer notes — 30 min spot vol dynamics

Do not share this file, `hidden/`, or `solution/` with the candidate.

## Setup (before they join)

```bash
make                          # candidate stub
./build/test_properties       # should FAIL on the stub
```

To sanity-check the exam itself:

```bash
make solution
./build-sol/test_properties
./build-sol/test_ssr_formula
./build-sol/print_smile
```

Keep `hidden/` and `solution/` closed in the editor. Candidate-facing files:

- `include/vol_surface.hpp`
- `include/bumped_vol_surface.hpp`
- `src/bumped_vol_surface.cpp`
- `README.md`
- optionally `tests/test_properties.cpp` (behavioural; does not give the formula)

## Script (~30 min)

**0–5 min — whiteboard.** “Spot jumps 1%. What happens to the implied vol smile?”

Steer toward two caricatures, then SSR:

| Rule | Smile sticks in | ATM vol after spot up (equity, negative skew) |
|---|---|---|
| Sticky strike | `K` | rides the skew, so ATM vol **drops** |
| Sticky delta | `K/F` (or delta) | ATM vol **unchanged** |
| SSR `λ` | interpolates | `dσ_ATM / d ln F = λ · (∂σ/∂k)|_{k=0}` |

Bergomi: equity index `λ` is typically 1.5–2, i.e. ATM vol moves *more* than
sticky-strike. SSR = 0 is sticky delta; SSR = 1 is sticky strike.

Mention that the live surface is an opaque `VolSurface` (forwards not equal
to spot — they must use `forward(t)`, not `K = 100`).

**5–10 min — read the API.** Constructor is already wired. They fill in
`forward` and `impliedVol`. Useful prompts if they stall:

- How do forwards move if `r` and `q` are unchanged?
- Sticky delta: which strike on the *old* surface has the same moneyness as
  `K` on the *new* surface?
- Can one formula cover both SSR = 0 and SSR = 1?

**10–25 min — code.** They edit `src/bumped_vol_surface.cpp`, rebuild, run
`./build/test_properties`. If they only nail SSR ∈ {0, 1} with an `if`, that
is a pass for 30 min; ask them to unify if time remains.

**25–30 min — wrap.** Pick from the extensions below depending on how far
they got.

## Reference implementation

See `solution/bumped_vol_surface.cpp`. Compact equivalent:

```cpp
forward(t) = u * base.forward(t);           // u = spot_multiplier

// σ'(k') = σ(k' + ssr * h),  h = ln u,  k' = ln(K / F')
K_old = K * pow(u, ssr - 1);
return base.impliedVol(K_old, t);
```

The log-moneyness form in the solution is easier to defend in discussion;
`K * u^{ssr-1}` is what you want in production.

## Rubric

| Signal | Strong | Weak |
|---|---|---|
| Forwards | Scales `base.forward(t)` by `u` | Hardcodes `S0`, forgets the bump, or uses `S+dS` additively |
| Sticky delta | Queries `K * F / F'` (or `K/u`) | Shifts ATM only; adds `dσ` by hand; uses `K=100` as ATM |
| SSR | One strike map, or ATM identity from the definition | Two unrelated special cases, no idea how 1.5 sits between them |
| C++ | `const`, no copy of the surface, `log`/`exp` vs `pow` | Slices the base surface, mutates it, ignores `t` |
| Discussion | Distinguishes sticky moneyness vs true Black delta; knows empirical SSR | Recites “sticky delta” without a testable property |

Passing bar: correct `forward`, correct sticky-delta `impliedVol`, and a
coherent explanation of sticky strike. Unifying via SSR is the stretch goal.

## Common mistakes

1. **ATM = 100.** The hidden SVI has `r ≠ q`, so `F(1) ≈ 102`. Tests use
   `forward(t)` as ATM; this mistake fails sticky-delta ATM invariance.
2. **Bump the vol, not the strike.** e.g. `σ' = σ + ssr * skew * dlnS` at
   every strike. That moves ATM but destroys the smile shape.
3. **Query the new forward on the old surface.** `base.impliedVol(K, t)` with
   `K` unchanged is sticky strike, not sticky delta.
4. **Wrong power.** `u^{ssr}` instead of `u^{ssr-1}` (off-by-one from mixing
   `k` vs `K`).
5. **True sticky delta.** Black delta is `N(d1)`, not `K/F`. Fine to mention;
   out of scope to implement. Sticky log-moneyness is the intended model.

## Extensions if they finish early

- **True sticky delta:** invert `Δ = N(d1(K, F, σ, t))` for `K`, holding delta
  fixed. Needs a solver and a convention (Payer/receiver, spot vs forward
  delta). Why the difference grows with skew and with `σ√T`.
- **`ssr(T)`:** short-dated equity SSR is larger (closer to local-vol / 2);
  long-dated closer to 1. How would the class change?
- **Floating vs sticky ATM.** Some desks slide the smile (SSR) *and* apply an
  independent ATM vol bump (a second risk). Two-parameter dynamics.
- **Where this is used.** Bumping the surface for a delta/vega book,
  sticky-delta vs sticky-strike P&L explain, mixing with local vol
  (Dupire) which implies SSR ≈ 2 for short dates (Bergomi’s rule of thumb).
- **Lifetime.** `const VolSurface&` vs `shared_ptr`; why the wrapper must not
  own a copy of a calibrated surface in a risk run.

## After the interview

Restore the stub before the next candidate:

```bash
git checkout -- src/bumped_vol_surface.cpp
```
