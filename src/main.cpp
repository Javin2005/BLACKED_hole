#include "raylib.h"

int main() {
    InitWindow(800, 450, "Svart Hål Simulering - Test");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircle(400, 225, 50, BLACK); 
        DrawText("Grattis! Raylib funkar.", 10, 10, 20, GREEN);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}