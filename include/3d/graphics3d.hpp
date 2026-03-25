#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <vector>
#include "photon3d.hpp"
#include "BlackHole3d.hpp"
#include "constants3d.hpp"
#include "raylib.h"

void DrawScene(const std::vector<Photon>& photons, const blackHole& bh, Camera3D& camera);

Color GetSpaceColor(Color baseColor, Vector3 velocity, Vector3 position,
                                 Vector3 cameraPos,float distance, float rs);
Color GetSpaceColor(Color baseColor, Vector3 velocity, float distance, float rs);
void DrawLine3DEx(Vector3 startPos, Vector3 endPos, float thickness, Color color);

#endif