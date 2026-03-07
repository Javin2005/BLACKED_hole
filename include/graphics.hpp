#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <vector>
#include "photon.hpp"
#include "BlackHole.hpp"
#include "constants.hpp"
#include <cmath>

void DrawScene(const std::vector<Photon>& photons, const blackHole& bh);

Color GetSpaceColor(Color baseColor, Vector2 velocity, float distance, float rs);

#endif