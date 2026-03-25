
 #include "raylib.h"
 #include "3d/photon3d.hpp"
 #include "3d/BlackHole3d.hpp"
 #include "3d/constants3d.hpp"
 #include "3d/physics3d.hpp"
 #include "3d/graphics3d.hpp"
 #include "3d/input3d.hpp"
 #include <vector>
 #include <algorithm>
 
 int main() {
     SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
     InitWindow(1280, 720, "Black Hole 3D");
 
     Camera3D camera = {0};
     camera.position   = {150.0f, 100.0f, 150.0f};
     camera.target     = {0.0f,   0.0f,   0.0f};
     camera.up         = {0.0f,   1.0f,   0.0f};
     camera.fovy       = 45.0f;
     camera.projection = CAMERA_PERSPECTIVE;
 
     SetTargetFPS(60);
 
     std::vector<Photon> photons;
     blackHole bh({0, 0, 0}, 10000.0);
 
     while (!WindowShouldClose()) {
         
         HandleInput(photons, bh, camera);
 
         
         if (photons.size() > 3000) {
             photons.erase(
                 std::remove_if(photons.begin(), photons.end(),
                     [](const Photon& p) { return !p.active; }),
                 photons.end()
             );
         }
 
        
         const int subSteps = 8;
         float dt = GetFrameTime();
         for (int i = 0; i < subSteps; i++) {
             UpdatePhysics(photons, bh, dt / subSteps, (i == 0));
         }
 
         DrawScene(photons, bh, camera);
     }
 
     CloseWindow();
     return 0;
 }