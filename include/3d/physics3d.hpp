#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <vector>
#include "photon3d.hpp"
#include "BlackHole3d.hpp"

void UpdatePhysics(std::vector<Photon>& photons, const blackHole& bh, float dt, bool saveHistory);
void SpawnDisk(std::vector<Photon>& photons, const blackHole& bh, int count);

#endif