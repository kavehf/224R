#pragma once

#include "vol_surface.hpp"

#include <memory>

// INTERVIEWER ONLY.
// Sample equity-like surface: S0 = 100, r = 3%, q = 1%, downward-skewed SVI
// slices. Forwards are *not* equal to spot, so ATM must be read at F(t).
std::unique_ptr<VolSurface> make_sample_surface();
