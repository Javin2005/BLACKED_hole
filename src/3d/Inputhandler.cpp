#include "InputHandler.hpp"
#include "Constants.hpp"
#include "raymath.h"
#include <cmath>

void InputHandler::updateCamera(Camera3D& camera) {
    
    if (IsKeyDown(KEY_L)) {
        Vector2 delta = GetMouseDelta();
        yaw_   -= delta.x * 0.003f;
        pitch_ -= delta.y * 0.003f;
        
        if (pitch_ >  1.55f) pitch_ =  1.55f;
        if (pitch_ < -1.55f) pitch_ = -1.55f;
    }

    
    Vector3 forward = {
        cosf(pitch_) * sinf(yaw_),
        sinf(pitch_),
        cosf(pitch_) * cosf(yaw_)
    };
    Vector3 right = {
        cosf(yaw_),
        0.0f,
        -sinf(yaw_)
    };

    
    float speed = IsKeyDown(KEY_LEFT_SHIFT) ? 8.0f : 2.0f;

    if (IsKeyDown(KEY_W)) camera.position = Vector3Add(camera.position, Vector3Scale(forward,  speed));
    if (IsKeyDown(KEY_S)) camera.position = Vector3Add(camera.position, Vector3Scale(forward, -speed));
    if (IsKeyDown(KEY_A)) camera.position = Vector3Add(camera.position, Vector3Scale(right,    speed));
    if (IsKeyDown(KEY_D)) camera.position = Vector3Add(camera.position, Vector3Scale(right,   -speed));
    if (IsKeyDown(KEY_E)) camera.position.y += speed;
    if (IsKeyDown(KEY_Q)) camera.position.y -= speed;

    camera.target = Vector3Add(camera.position, forward);

    
    float scroll = GetMouseWheelMove();
    if (scroll != 0.0f) {
        camera.fovy = fmaxf(10.0f, fminf(120.0f, camera.fovy - scroll * 3.0f));
    }
}

void InputHandler::handleInput(Simulation& sim, Camera3D& camera) {
    updateCamera(camera);

    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Ray ray = GetMouseRay(GetMousePosition(), camera);
        Vector3 vel = Vector3Scale(ray.direction, Physics::PHOTON_SPEED);
        sim.spawnPhoton(ray.position, vel, WHITE, true);
    }

    if (IsKeyPressed(KEY_SPACE))  sim.clearPhotons();
    if (IsKeyDown(KEY_G))         sim.spawnDisk(30);
    if (IsKeyDown(KEY_UP))        sim.getBlackHole().addMass(100.0);
    if (IsKeyDown(KEY_DOWN))      sim.getBlackHole().addMass(-100.0);

    
    if (IsKeyPressed(KEY_R)) {
        camera.position = {150.0f, 100.0f, 150.0f};
        camera.fovy     = 45.0f;
        yaw_   = -2.35f;
        pitch_ = -0.3f;
        camera.target = Vector3Add(camera.position, Vector3{
            cosf(pitch_)*sinf(yaw_), sinf(pitch_), cosf(pitch_)*cosf(yaw_)
        });
    }

    if (IsKeyPressed(KEY_F)) {
        if (IsWindowFullscreen()) SetWindowSize(1280, 720);
        else { int m = GetCurrentMonitor(); SetWindowSize(GetMonitorWidth(m), GetMonitorHeight(m)); }
        ToggleFullscreen();
    }
}