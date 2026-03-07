#include "input.hpp"
#include "physics.hpp"
#include <vector>
#include "photon.hpp"
#include "BlackHole.hpp"



void HandleInput(std::vector<Photon>& photons, blackHole& bh){
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        photons.push_back({ GetMousePosition(), {200,0}, {},true, WHITE});
    }
    if(IsKeyPressed(KEY_SPACE)){
        photons.clear();
    }
    if(IsKeyDown(KEY_UP)) {
        bh.mass += 20;
        bh.eventHorizonRadius = (double)((2.0 * G * bh.mass) / (C * C));
    }

    if(IsKeyDown(KEY_DOWN) && bh.mass > 100) {
    bh.mass -= 20;
    bh.eventHorizonRadius = (double)((2.0 * G * bh.mass) / (C * C));
    }
    if(IsKeyPressed(KEY_D)){
        SpawnDisk(photons,bh,50);
    }

    if (IsKeyDown(KEY_P)) {
        bh.position = GetMousePosition();
    }
}