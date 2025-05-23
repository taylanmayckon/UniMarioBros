#include "raylib.h"

int main(void){
    InitWindow(800, 450, "UniMarioBros");
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Mario", 190, 200, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
