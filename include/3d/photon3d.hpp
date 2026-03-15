#ifndef PHOTON_HPP
#define PHOTON_HPP

#include "raylib.h"
#include <vector>


constexpr int MAX_HISTORY = 100;

struct Photon {
    Vector3 position;
    Vector3 velocity;
    Vector3 history[MAX_HISTORY];
    int historyIndex = 0;
    int historyCount = 0;
    bool active;
    Color baseColor;
    bool body;

    void savePosition() {
        history[historyIndex] = position;
        historyIndex = (historyIndex + 1) % MAX_HISTORY; 
        if (historyCount < MAX_HISTORY) historyCount++;
    }
};



#endif