#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "raylib.h"
//märkte det var lite crazy att börja med particlar redan men sen kanske
struct Particle {
    Vector3 position;
    Vector3 velocity;
    double mass;
    bool active;
};

#endif