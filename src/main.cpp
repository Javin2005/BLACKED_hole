#include "raylib.h"
#include "photon.hpp"
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

    while (!WindowShouldClose()) {

        for(auto& photon : photons) {
            if (photon.active) {
                photon.position.x += photon.velocity.x;
                photon.position.y += photon.velocity.y;

            }
        }


        BeginDrawing();
        ClearBackground(BLACK);

        for(auto& photon: photons) {
            if(photon.active) {
                DrawCircleV(photon.position,10,YELLOW);
            }
        }

        EndDrawing();

    }


    return 0;
}