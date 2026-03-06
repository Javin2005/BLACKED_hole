#ifndef blackHole_HPP
#define blackHole_HPP

#include "raylib.h"

struct blackHole {
    Vector2 position;
    double mass;
    float eventHorizonRadius;
    bool active;
};


#endif
