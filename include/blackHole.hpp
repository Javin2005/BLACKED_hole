#ifndef blackHole_HPP
#define blackHole_HPP

#include "raylib.h"
#include "constants.hpp"


struct blackHole {
    Vector2 position;
    double mass;
    float eventHorizonRadius;
    bool active;

    void updateRadius() {
        eventHorizonRadius = (float)((2.0 * G * mass) / (C * C));
    }
};


#endif
