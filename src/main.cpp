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
        tempPhoton.position = { (float)(100 + 50* pow(-1,i)) , (float)(200 + i * 50) }; 
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

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Photon newPhoton;
            newPhoton.position = GetMousePosition();
            newPhoton.velocity = {3,0};
            newPhoton.active = true;
            photons.push_back(newPhoton);
        }
        if(IsKeyPressed(KEY_SPACE)){
            photons.clear();
        }


        for(auto& photon : photons) {
            if (photon.active) {
                Vector2 direction = Vector2Subtract(bh.position, photon.position);
                float distance = Vector2Length(direction);


                if (distance < 10*bh.eventHorizonRadius) { 
                    photon.active = false;
                    continue;
                }

                float force = (bh.mass)*G / (distance * distance); 

                Vector2 acceleration = Vector2Scale(Vector2Normalize(direction), force);
                photon.velocity = Vector2Add(photon.velocity, acceleration);
                photon.position = Vector2Add(photon.position, photon.velocity);

            }

            if (Vector2Length(photon.velocity) > C) {
                photon.velocity = Vector2Scale(Vector2Normalize(photon.velocity), (float)C);
            }

            if(photon.position.x < 0 || photon.position.x > GetScreenWidth() || 
            photon.position.y < 0 || photon.position.y > GetScreenHeight()) {
                photon.active = false;
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