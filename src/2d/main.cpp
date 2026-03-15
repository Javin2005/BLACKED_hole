#include "raylib.h"
#include "2d/photon.hpp"
#include "2d/BlackHole.hpp"
#include "2d/constants.hpp"
#include "2d/physics.hpp"   
#include "2d/graphics.hpp" 
#include "2d/input.hpp" 
#include <vector>
#include <string>
#include <algorithm>



int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);


    InitWindow(800, 800, "Blacked hole 3");

    SetTargetFPS(60);

    std::vector<Photon> photons;
    
    blackHole bh({400, 400}, 10000.0);
    
    while (!WindowShouldClose()) {
        if (IsWindowResized()) {
            bh.position = { (float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f };
        }

        HandleInput(photons,bh);
        DrawText(TextFormat("Mass: %.0f", (float)bh.mass), 10, 10, 20, GREEN);
       
        if(photons.size() > 1000) {
            photons.erase(std::remove_if(photons.begin(), photons.end(), 
                            [](const Photon& p) { return !p.active;}), photons.end());
        }
        

        int subSteps = 8;
        float dt = 1.0f / 60.0f; 
        for (int i = 0; i < subSteps; i++) {
            UpdatePhysics(photons, bh, dt / subSteps, (i == 0)); 
        }
        DrawScene(photons, bh);

        
        
        

        

    }

    CloseWindow();
    return 0;
}