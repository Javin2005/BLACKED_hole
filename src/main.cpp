#include "raylib.h"
#include "photon.hpp"
#include "BlackHole.hpp"
#include "constants.hpp"
#include "physics.hpp"   
#include "graphics.hpp"  
#include <vector>

int main() {
    InitWindow(800, 800, "Blacked hole");

    SetTargetFPS(60);

    std::vector<Photon> photons;

    
    for (int i = 0; i < 10; i++) {
        photons.push_back({ {(float)(100 + 50*pow(-1,i)), (float)(200 + i*50*pow(-1,i))}, {3, 0}, {}, true});
    }

    blackHole bh = { {400, 400}, 2000.0, 0, true };
    bh.eventHorizonRadius = (double)((2.0 * 100 * G * bh.mass) / (C * C));

    while (!WindowShouldClose()) {

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            photons.push_back({ GetMousePosition(), {3,0}, {},true});
        }
        if(IsKeyPressed(KEY_SPACE)){
            photons.clear();
        }


        UpdatePhysics(photons, bh);
        DrawScene(photons, bh);

        

    }


    return 0;
}