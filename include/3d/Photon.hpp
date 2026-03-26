#pragma once
#include "raylib.h"



constexpr int MAX_HISTORY = 150;


struct Photon {
    Vector3 position;
    Vector3 velocity;
    Vector3 history[MAX_HISTORY];
    int historyIndex = 0;
    int historyCount = 0;
    bool active = false;
    bool body = false;
    Color baseColor = WHITE;



    inline void savePosition() {
        history[historyIndex] = position;
        historyIndex = (historyIndex + 1) % MAX_HISTORY;
        if ( historyCount < MAX_HISTORY) historyCount++;
    };

    void reset( Vector3 pos, Vector3 vel, Color color, bool isBody) {

        position     = pos;
        velocity     = vel;
        baseColor    = color;
        body         = isBody;
        active       = true;
        historyIndex = 0;
        historyCount = 0;
    };


};



