#ifndef blackHole_HPP
#define blackHole_HPP

#include "raylib.h"
#include "constants.hpp"


struct blackHole {
    Vector2 position;
    double mass;
    float eventHorizonRadius;
    bool active;

    
    blackHole(Vector2 pos, double m) : position(pos), mass(m), active(true) {
        updateRadius();
    }

    void updateRadius() {
        eventHorizonRadius = (float)((2.0 * G * mass) / (C * C));
    }

    void addMass(double amount) {
        mass += amount;
        if (mass < 100) mass = 100;
        updateRadius(); 
    }
};


#endif
