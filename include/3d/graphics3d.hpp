#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <vector>
#include "photon3d.hpp"
#include "BlackHole3d.hpp"
#include "constants3d.hpp"
#include <cmath>

void DrawScene(const std::vector<Photon>& photons, const blackHole& bh, Camera3D& camera);

Color GetSpaceColor(Color baseColor, Vector3 velocity, float distance, float rs);

#endif