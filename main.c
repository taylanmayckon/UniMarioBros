#include <stdio.h>
#include "raylib.h"
#include "mario.h"
#include "scene.h"

// Inicializando Mario_t
Mario_t Mario;

int main(void){
    InitWindow(800, 600, "UniMarioBros");

    SetTargetFPS(60);

    GameScene scene = CreateGameScene("assets/textures/background1.png"); // Função da biblioteca Scene para configurar o cenário

    InitMario(&Mario); // Inicializando as structs do Mario com valores
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawGameScene(scene); // Função da biblioteca Scene para desenhar o cenário
        
        DrawMario(&Mario);
        EndDrawing();
    }

    UnloadGameScene(scene); // Função da biblioteca Scene para liberar os recursos alocados dinamicamente
    CloseWindow();
    return 0;
}
