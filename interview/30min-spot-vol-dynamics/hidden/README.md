INTERVIEWER ONLY — do not open this directory in a candidate session.

Contains:
- SVI term-structure vol surface (`svi_vol_surface.*`)
- `make_sample_surface()` factory used by tests and the smile demo
- formula-level SSR check (`test_ssr_formula.cpp`) that would give away
  the implementation if the candidate ran it

The candidate-facing tests in `../tests/` only assert *behaviour*
(sticky-delta moneyness invariance, sticky-strike K invariance, and the
first-order ATM identity that *defines* SSR).
