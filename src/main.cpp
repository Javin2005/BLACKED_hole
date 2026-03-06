#include "raylib.h"
#include "photon.hpp"
#include "BlackHole.hpp"
#include "constants.hpp"
#include "physics.hpp"   
#include "graphics.hpp"  
#include <vector>
#include <string>


int main() {
    InitWindow(800, 800, "Blacked hole");

    SetTargetFPS(60);

    std::vector<Photon> photons;

    
    for (int i = 0; i < 10; i++) {
        photons.push_back({ {(float)(100 + 50*pow(-1,i)), (float)(200 + i*50*pow(-1,i))}, {3, 0}, {}, true, WHITE});
    }

    blackHole bh = { {400, 400}, 2000.0, 0, true };
    bh.eventHorizonRadius = (double)((2.0 * 100 * G * bh.mass) / (C * C));

    while (!WindowShouldClose()) {

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            photons.push_back({ GetMousePosition(), {3,0}, {},true, WHITE});
        }
        if(IsKeyPressed(KEY_SPACE)){
            photons.clear();
        }
        if(IsKeyDown(KEY_UP)) bh.mass += 20;
        if(IsKeyDown(KEY_DOWN) && bh.mass > 100) bh.mass -= 20;

        if(IsKeyPressed(KEY_D)){
            SpawnDisk(photons,bh,20);
        }

        bh.eventHorizonRadius = (double)((2.0 * 100 * G * bh.mass) / (C * C));


        DrawText(std::to_string(bh.mass).data(), 10, 10, 20, GREEN);


        int subSteps = 8;
        float dt = 1.0f / 60.0f; 
        for (int i = 0; i < subSteps; i++) {
            UpdatePhysics(photons, bh, dt / subSteps); 
        }


        DrawScene(photons, bh);

        

    }


    return 0;
}