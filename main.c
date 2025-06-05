#include <stdio.h>
#include "raylib.h"
#include "mario.h"
#include "scene.h"

// Inicializando Mario_t
Mario_t Mario;

// Variavel de tempo para testes
float timer = 0.0f;

int main(void){
    InitWindow(990, 720, "UniMarioBros");

    SetTargetFPS(60);

    GameScene scene = CreateGameScene("assets/textures/background.png"); // Função da biblioteca Scene para configurar o cenário

    InitMario(&Mario); // Inicializando as structs do Mario com valores
    // Mario.actualState = ACTION_WALKING;
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawGameScene(scene); // Função da biblioteca Scene para desenhar o cenário
        UpdateMario(&Mario);
        
        DrawMario(&Mario);
        EndDrawing();
    }

    UnloadGameScene(scene); // Função da biblioteca Scene para liberar os recursos alocados dinamicamente
    CloseWindow();
    return 0;
}
