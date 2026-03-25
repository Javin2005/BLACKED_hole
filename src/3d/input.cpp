#include "3d/input3d.hpp"
#include "3d/physics3d.hpp"
#include <vector>
#include "3d/photon3d.hpp"
#include "3d/BlackHole3d.hpp"
#include "raymath.h"
#include <cmath>




void UpdateFreeCamera(Camera3D& camera) {

    if(IsKeyDown(KEY_L)) {
        Vector2 delta = GetMouseDelta();

        float yaw = -delta.x * 0.003f;

        float pitch = -delta.y * 0.003f;

        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

        Quaternion qYaw   = QuaternionFromAxisAngle({0, 1, 0}, yaw);
        Quaternion qPitch = QuaternionFromAxisAngle(right, pitch);
        Quaternion q      = QuaternionMultiply(qYaw, qPitch);

        forward = Vector3RotateByQuaternion(forward, q);


        float dot = Vector3DotProduct(forward, {0, 1, 0});
        if (fabsf(dot) < 0.99f) {
            camera.target = Vector3Add(camera.position, forward);
        }

    }

    
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, {0, 1, 0}));
    
 
    float baseSpeed = 2.0f;
    float speed     = IsKeyDown(KEY_LEFT_SHIFT) ? baseSpeed * 5.0f : baseSpeed;
 
    Vector3 move = {0, 0, 0};
    if (IsKeyDown(KEY_W)) move = Vector3Add(move, Vector3Scale(forward, speed));
    if (IsKeyDown(KEY_S)) move = Vector3Add(move, Vector3Scale(forward, -speed));
    if (IsKeyDown(KEY_D)) move = Vector3Add(move, Vector3Scale(right, speed));
    if (IsKeyDown(KEY_A)) move = Vector3Add(move, Vector3Scale(right, -speed));
    if (IsKeyDown(KEY_E)) move.y += speed;  
    if (IsKeyDown(KEY_Q)) move.y -= speed;   
 
    camera.position = Vector3Add(camera.position, move);
    camera.target   = Vector3Add(camera.target, move);
 
    
    float scroll = GetMouseWheelMove();
    if (scroll != 0.0f) {
        camera.fovy -= scroll * 2.0f;
        camera.fovy  = fmaxf(10.0f, fminf(120.0f, camera.fovy));
    }
}







void HandleInput(std::vector<Photon>& photons, blackHole& bh, Camera3D& camera) {
 
   
    UpdateFreeCamera(camera);
 
    
    if (IsKeyDown(KEY_K)) {
        
        Ray ray = GetMouseRay(GetMousePosition(), camera);
 
        
        Vector3 velocity = Vector3Scale(ray.direction, (float)C);
 
        photons.push_back({ray.position,velocity,{},0, 0,true,WHITE,true         
        });
    }
 
  
    if (IsKeyPressed(KEY_SPACE)) {
        photons.clear();
    }
 
   
    if (IsKeyDown(KEY_UP))   bh.addMass(50.0);
    if (IsKeyDown(KEY_DOWN)) bh.addMass(-50.0);
 
   
    if (IsKeyDown(KEY_P)) {
        SpawnDisk(photons, bh, 30);
    }
 
   
    if (IsKeyPressed(KEY_F)) {
        if (IsWindowFullscreen()) {
            SetWindowSize(1280, 720);
        } else {
            int monitor = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        }
        ToggleFullscreen();
    }
 
    
    if (IsKeyPressed(KEY_R)) {
        camera.position = {150.0f, 100.0f, 150.0f};
        camera.target   = {0.0f,   0.0f,   0.0f};
        camera.fovy     = 45.0f;
    }
}