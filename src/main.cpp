#include "raylib.h"

int main() {
    InitWindow(800, 450, "Svart Hål Simulering - Test");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircle(400, 225, 50, BLACK);
        DrawCircle(300, 225, 5, GREEN);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}