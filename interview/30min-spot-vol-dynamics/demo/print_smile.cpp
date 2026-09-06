#include "bumped_vol_surface.hpp"
#include "surface_factory.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    const auto base = make_sample_surface();
    const double u = 1.02;  // +2% spot
    const BumpedVolSurface sticky_delta(*base, u, /*ssr=*/0.0);
    const BumpedVolSurface sticky_strike(*base, u, /*ssr=*/1.0);
    const BumpedVolSurface ssr15(*base, u, /*ssr=*/1.5);

    const double t = 1.0;
    const double F = base->forward(t);
    const double Fp = sticky_delta.forward(t);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "T = " << t << "   F = " << F << "   F' = " << Fp
              << "   u = " << u << "\n\n";
    std::cout << "Implied vol at fixed moneyness m = K/F "
              << "(bumped surfaces evaluated at K = m * F'):\n\n";
    std::cout << "      m         K     base   sticky-delta  sticky-strike  SSR=1.5\n";

    for (double m : std::vector<double>{0.80, 0.90, 1.00, 1.10, 1.25}) {
        const double K = m * F;
        const double Kp = m * Fp;
        std::cout << std::setw(8) << m << std::setw(10) << K
                  << std::setw(9) << base->impliedVol(K, t)
                  << std::setw(14) << sticky_delta.impliedVol(Kp, t)
                  << std::setw(15) << sticky_strike.impliedVol(Kp, t)
                  << std::setw(10) << ssr15.impliedVol(Kp, t) << "\n";
    }

    std::cout << "\nATM (m = 1) on each surface:\n";
    std::cout << "  base            " << base->impliedVol(F, t) << "\n";
    std::cout << "  sticky-delta    " << sticky_delta.impliedVol(Fp, t)
              << "   (matches base: smile stuck in K/F)\n";
    std::cout << "  sticky-strike   " << sticky_strike.impliedVol(Fp, t)
              << "   (spot up, equity skew => ATM vol down)\n";
    std::cout << "  SSR=1.5         " << ssr15.impliedVol(Fp, t)
              << "   (moves more than sticky-strike)\n";
    return 0;
}
