#ifndef PHOTON_HPP
#define PHOTON_HPP

#include "raylib.h"
#include <vector>

struct Photon {
    Vector2 position;
    Vector2 velocity;
    std::vector<Vector2> history;
    bool active;
    Color baseColor;
    bool body;
};



#endif