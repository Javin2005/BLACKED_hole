#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <vector>
#include "photon.hpp"
#include "BlackHole.hpp"

void UpdatePhysics(std::vector<Photon>& photons, const blackHole& bh);

#endif