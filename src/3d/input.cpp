#include "3d/input3d.hpp"
#include "3d/physics3d.hpp"
#include <vector>
#include "3d/photon3d.hpp"
#include "3d/BlackHole3d.hpp"
#include "raymath.h"



void HandleInput(std::vector<Photon>& photons, blackHole& bh, Camera3D& camera) {
    
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        
        Ray ray = GetMouseRay(GetMousePosition(), camera);
        
       
        float speed = 10.0f; 
        Vector3 velocity = Vector3Scale(ray.direction, speed);
        
        
        photons.push_back({ 
            ray.position, 
            velocity,     
            {0,0,0},      
            0, 0,         
            true, WHITE, true 
        });
    }

    if (IsKeyPressed(KEY_SPACE)) {
        photons.clear();
    }

    
    if (IsKeyDown(KEY_UP)) bh.addMass(100);
    if (IsKeyDown(KEY_DOWN)) bh.addMass(-100);

    
    if (IsKeyPressed(KEY_D)) {
        SpawnDisk(photons, bh, 50);
    }


    if (IsKeyDown(KEY_P)) {
        Ray ray = GetMouseRay(GetMousePosition(), camera);
        bh.position = Vector3Add(ray.position, Vector3Scale(ray.direction, 10.0f));
    }

    
    if (IsKeyPressed(KEY_F)) {
        if (IsWindowFullscreen()) {
            SetWindowSize(800, 800);
        } else {
            int monitor = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        }
        ToggleFullscreen();
    }
}