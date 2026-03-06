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
                float maxThickness = 5.0f;
                for (size_t i = 0; i < photon.history.size() - 1; i+=1) {
                    float thickness = maxThickness * ((float)i / photon.history.size());
                    Color c = Fade(photon.color, (float)i / photon.history.size());
                    DrawLineEx(photon.history[i], photon.history[i + 1], thickness, c);
                }
                DrawLineEx(photon.history.back(), photon.position, maxThickness, photon.color);

                
            }

            DrawCircleV(photon.position, 5, photon.color);
            
        }
    }


    EndDrawing();
}