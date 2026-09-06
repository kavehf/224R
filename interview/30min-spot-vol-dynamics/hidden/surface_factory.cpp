#include "surface_factory.hpp"

#include "svi_vol_surface.hpp"

#include <vector>

std::unique_ptr<VolSurface> make_sample_surface() {
    // Roughly equity-index: negative rho, mild term structure of ATM vol.
    // ATM vols (approx): 1m ~ 18%, 3m ~ 17%, 1y ~ 16.5%, 2y ~ 16%.
    const std::vector<SviSlice> slices = {
        {1.0 / 12.0, 0.0024, 0.055, -0.55, 0.00, 0.08},
        {0.25,       0.0060, 0.085, -0.60, 0.00, 0.10},
        {1.00,       0.0200, 0.125, -0.65, -0.04, 0.12},
        {2.00,       0.0360, 0.155, -0.70, -0.06, 0.14},
    };
    return std::make_unique<SviVolSurface>(/*spot=*/100.0,
                                           /*rate=*/0.03,
                                           /*dividend=*/0.01,
                                           slices);
}
