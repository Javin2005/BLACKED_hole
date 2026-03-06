#include "graphics.hpp"
#include "raylib.h"
#include "photon.hpp"

/** 
 * Draws a Scene(frame) of the space we see
 * @param photons - a mutable vector of the active and unactive photons
 * @param blackHole - a blackHole object
 * 
 * */ 
void DrawScene(const std::vector<Photon>& photons, const blackHole& bh){
    BeginDrawing();
    ClearBackground(BLACK);

    DrawCircleV(bh.position, bh.eventHorizonRadius + 5, GRAY);
    DrawCircleV(bh.position, bh.eventHorizonRadius, BLACK);

    for(const auto& photon : photons) {
        if(photon.active) {
            if (photon.history.size() > 1) {
                DrawLineStrip(photon.history.data(), (int)photon.history.size(), WHITE);
            }
            
            DrawCircleV(photon.position, 5, WHITE);
            
        }
    }


    EndDrawing();
}