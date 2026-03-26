#pragma once


#include "Simulation.hpp"
#include "raylib.h"



class InputHandler {
public:
    void handleInput(Simulation& sim, Camera3D& camera);

private:
    void updateCamera(Camera3D& camera);
    float yaw_   = -2.35f;   //<-- gissingar på rotation not to self testa tills det fungerar
    float pitch_ = -0.3f;

};