#include "raylib.h"
#include "photon.hpp"
#include "BlackHole.hpp"
#include "constants.hpp"
#include "physics.hpp"   
#include "graphics.hpp" 
#include "input.hpp" 
#include <vector>
#include <string>
#include <algorithm>



int main() {
    InitWindow(800, 800, "Blacked hole");

    SetTargetFPS(60);

    std::vector<Photon> photons;

    
    for (int i = 0; i < 10; i++) {
        photons.push_back({ {(float)(100 + 50*pow(-1,i)), (float)(200 + i*50*pow(-1,i))}, {250, 0}, {}, true, WHITE,true});
    }

    blackHole bh = { {400, 400}, 10000.0, 0, true };
    bh.eventHorizonRadius = (double)((2.0 * G * bh.mass) / (C * C));

    while (!WindowShouldClose()) {

        HandleInput(photons,bh);
       
        if(photons.size() > 1000) {
            photons.erase(std::remove_if(photons.begin(), photons.end(), 
                            [](const Photon& p) { return !p.active;}), photons.end());
        }
        


        DrawText(std::to_string(bh.mass).data(), 10, 10, 20, GREEN);


        int subSteps = 8;
        float dt = 1.0f / 60.0f; 
        for (int i = 0; i < subSteps; i++) {
            UpdatePhysics(photons, bh, dt / subSteps, (i == 0)); 
        }


        DrawScene(photons, bh);

        

    }


    return 0;
}