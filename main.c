#include <stdio.h>
#include "raylib.h"
#include "mario.h"
#include "scene.h"
#include "platform.h"
#include "camera.h"
#include "coin.h"

#define SCREEN_WIDTH 990
#define SCREEN_HEIGHT 720

// Inicializando Mario_t
Mario_t Mario;
// Inicializando a câmera
Camera2D gameCamera;
// Inicializando o vetor plataformas dinâmicas
PhysPlatform_t physPlatforms[MAX_PHYS_PLATFORMS];

// Variavel de tempo para testes
float timer = 0.0f;

// Placeholders para não bugar compilação
Sound bumpSound;
Sound jumpSound;

int main(void){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "UniMarioBros");
    SetTargetFPS(60);
    GameScene scene = CreateGameScene("assets/textures/background.png"); // Função da biblioteca Scene para configurar o cenário
    // (PLACHOLDER) bloco para teste de DrawBlocks
    Texture2D block1 = LoadTexture("assets/textures/blocks/block1.png");

    InitMario(&Mario); // Inicializando as structs do Mario com valores
    InitPlatforms(physPlatforms); // Inicializando as plataformas
    InitCamera(&gameCamera, SCREEN_WIDTH, SCREEN_HEIGHT); // Inicializando a câmera

    while (!WindowShouldClose()) {
        // Processando o back
        UpdateMario(&Mario); // Movimentação, física e preparação de output para outras libs
        HandleMarioPlatformCollisions(&Mario, physPlatforms, bumpSound); // Atualiza a colisão do Mario
        UpdatePlatforms(physPlatforms);
        UpdateCoins(&Mario); // Atualiza as moedas
        MoveCamera(&gameCamera, Mario, SCREEN_WIDTH, SCREEN_HEIGHT); // Atualiza os parâmetros da câmera

        // Processando o front
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(gameCamera);        

        DrawGameScene(scene); // Função da biblioteca Scene para desenhar o cenário
        DrawCoins(); // Desenha moedas
        DrawBlocks(physPlatforms, block1);
        DrawMario(&Mario); // Atualiza a posição do desenho do Mario

        EndMode2D();
        EndDrawing();
    }

    UnloadGameScene(scene); // Função da biblioteca Scene para liberar os recursos alocados dinamicamente
    CloseWindow();
    return 0;
}
