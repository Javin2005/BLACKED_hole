#include "raylib.h"
#include "Simulation.hpp"
#include "InputHandler.hpp"


int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(1280,720, "Black Hole 3D");
    SetTargetFPS(60);

    Camera3D camera = {{ 150, 100, 150 }, { 0, 0, 0 }, { 0, 1, 0 }, 45.0f, CAMERA_PERSPECTIVE };
    Simulation sim ({0,0,0});
    InputHandler input;


    while (!WindowShouldClose()) {
        input.handleInput(sim, camera);
        sim.update(camera);
        sim.draw(camera);
    }


    CloseWindow();
    return 0;


}