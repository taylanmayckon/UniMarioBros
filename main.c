#include "raylib.h"
#include "mario.h"



int main(void){
    InitWindow(800, 450, "UniMarioBros");
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Mario", 190, 200, 20, BLACK);
        DrawMario();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
