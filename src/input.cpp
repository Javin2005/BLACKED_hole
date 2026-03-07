#include "input.hpp"
#include "physics.hpp"
#include <vector>
#include "photon.hpp"
#include "BlackHole.hpp"



void HandleInput(std::vector<Photon>& photons, blackHole& bh){
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        photons.push_back({ GetMousePosition(), {200, 0}, {}, 0, 0, true, WHITE, true });
    }
    if(IsKeyPressed(KEY_SPACE)){
        photons.clear();
    }
    if(IsKeyDown(KEY_UP)) {
        bh.addMass(20);
    }

    if(IsKeyDown(KEY_DOWN)) {
        bh.addMass(-20);
    }
    if(IsKeyPressed(KEY_D)){
        SpawnDisk(photons,bh,50);
    }

    if (IsKeyDown(KEY_P)) {
        bh.position = GetMousePosition();
    }
}