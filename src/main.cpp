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

    Photon p;
    p.position = {100,400};
    p.velocity = {2,0};
    p.active = true;

    photons.push_back(p);

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


                if (distance < 20.0f) { 
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
        DrawCircleV(bh.position, 55, GRAY);
        DrawCircleV(bh.position,50,BLACK);

        for(auto& photon: photons) {
            if(photon.active) {
                DrawCircleV(photon.position,10,YELLOW);
            }
        }

        EndDrawing();

    }


    return 0;
}