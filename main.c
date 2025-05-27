#include <stdio.h>
#include "raylib.h"
#include "mario.h"

// Inicializando Mario_t
Mario_t Mario;

int main(void){
    InitWindow(800, 450, "UniMarioBros");

    SetTargetFPS(60);

    InitMario(&Mario); // Inicializando as structs do Mario com valores
    
    while (!WindowShouldClose()) {
        //UpdateMarioSprite(&Mario);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawMario(&Mario);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
