#include "2d/input.hpp"
#include "2d/physics.hpp"
#include <vector>
#include "2d/photon.hpp"
#include "2d/BlackHole.hpp"



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

    if(IsKeyPressed(KEY_F)){
        int display = GetCurrentMonitor();

        if (IsWindowFullscreen()) {
            SetWindowSize(800, 800); 
        } 
        else {
            SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        }
        ToggleFullscreen();
    }
}