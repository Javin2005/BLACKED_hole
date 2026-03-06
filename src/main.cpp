#include "raylib.h"
#include "photon.hpp"
#include "BlackHole.hpp"
#include "constants.hpp"
#include "raymath.h"
#include <vector>

int main() {
    InitWindow(800, 800, "Blacked hole");

    SetTargetFPS(60);

    std::vector<Photon> photons;

    
    for (int i = 0; i < 10; i++) {
        Photon tempPhoton;
        tempPhoton.position = { 100, (float)(200 + i * 50) }; 
        tempPhoton.velocity = { 3, 0 };                       
        tempPhoton.active = true;
        
        photons.push_back(tempPhoton);
    }

    blackHole bh;
    bh.position = { 400, 400 };
    bh.mass = 2000.0;

    bh.eventHorizonRadius = (float)((2.0 * G * bh.mass) / (C * C));
    bh.active = true;

    while (!WindowShouldClose()) {

        

        for(auto& photon : photons) {
            if (photon.active) {
                Vector2 direction = Vector2Subtract(bh.position, photon.position);
                float distance = Vector2Length(direction);


                if (distance < bh.eventHorizonRadius) { 
                    photon.active = false;
                    continue;
                }

                float force = (bh.mass)*G / (distance * distance); 

                Vector2 acceleration = Vector2Scale(Vector2Normalize(direction), force);
                photon.velocity = Vector2Add(photon.velocity, acceleration);
                photon.position = Vector2Add(photon.position, photon.velocity);

            }
        }


        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircleV(bh.position, bh.eventHorizonRadius + 5, GRAY);
        DrawCircleV(bh.position,bh.eventHorizonRadius,BLACK);

        for(auto& photon: photons) {
            if(photon.active) {
                DrawCircleV(photon.position,10,YELLOW);
            }
        }

        EndDrawing();

    }


    return 0;
}