#include <stdio.h>
#include "raylib.h"
#include "mario.h"

// Inicializando Mario_t
Mario_t Mario;

// Variavel de tempo para testes
float timer = 0.0f;

int main(void){
    InitWindow(800, 600, "UniMarioBros");

    SetTargetFPS(60);

    GameScene scene = CreateGameScene("assets/textures/background1.png"); // Função da biblioteca Scene para configurar o cenário

    InitMario(&Mario); // Inicializando as structs do Mario com valores
    
    while (!WindowShouldClose()) {
        

        // Estrutura basica pra testar troca de animação
        if(timer<=5.0f){
           timer += GetFrameTime();
        }
        else{
            timer = 0.0f;
            Mario.facingRight = !Mario.facingRight;
        }

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
