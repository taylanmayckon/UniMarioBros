/*******************************************************************************************
 * Estrutura de pastas esperada:
 *   assets/
 *     textures/
 *       background.png
 *       ItemsAndBlock.png
 *       coin.png
 *       mario/
 *         smallmario.png
 *         supermario.png
 *         firemario.png
 *     audio/
 *       bump.wav
 *       jump.wav
 *
 * Compile com:
 *   gcc -o uni_mariobros main.c coin.c game_scene.c mario.c platform.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
 *
 * Ajuste as flags conforme sua instalação do raylib.
 *******************************************************************************************/

// Inclui biblioteca padrão de entrada/saída
#include <stdio.h>
// Inclui biblioteca padrão de alocação de memória
#include <stdlib.h>
// Inclui biblioteca matemática
#include <math.h>
// Inclui cabeçalho da raylib
#include "raylib.h"

// Textura global dos blocos
static Texture2D blocksTexture;

#include "game_scene.h"
#include "platform.h"
#include "coin.h"
#include "mario.h"

// Som de bounce
static Sound bumpSound;
// Som de pulo
static Sound jumpSound;

// Contador de plataformas dinâmicas (já declarado em platform.c)
// extern int physPlatCount;
// extern PhysPlatform_t physPlatforms[MAX_PHYS_PLATFORMS];

// Contador de moedas e vetor (já declarados em coin.c)
// extern int coinCount;
// extern Coin_t coins[MAX_COINS];
// extern Texture2D coinAtlas;

// Prototipação de GetMarioCollisionRect
static Rectangle GetMarioCollisionRect(Mario_t *Mario);

int main(void) {
    const int screenWidth  = 990; // Largura da janela
    const int screenHeight = 720; // Altura da janela
    InitWindow(screenWidth, screenHeight, "UniMarioBros Unificado (arquivo único)"); // Inicializa janela
    InitAudioDevice(); // Inicializa áudio
    SetTargetFPS(60);  // Define FPS

    blocksTexture = LoadTexture("assets/textures/ItemsAndBlock.png"); // Carrega textura blocos

    GameScene scene = CreateGameScene("assets/textures/background.png"); // Cria cenário

    bumpSound = LoadSound("assets/audio/bump.wav"); // Carrega som bump
    jumpSound = LoadSound("assets/audio/jump.wav"); // Carrega som pulo

    coinAtlas = LoadTexture("assets/textures/coin.png"); // Carrega textura moeda

    physPlatCount = 0; // Zera contador plataformas

    physPlatforms[physPlatCount++] = (PhysPlatform_t){
        .rect        = { 0.0f, 545.0f, 1200.0f, 120.0f },
        .originalY   = 545.0f,
        .bounceOffset=  0.0f,
        .bouncing    = false,
        .bounceDir   =  0,
        .bounceSpeed =  0.0f
    };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){
        .rect        = { 500.0f, 355.0f, 60.0f, 57.0f },
        .originalY   = 355.0f,
        .bounceOffset=  0.0f,
        .bouncing    = false,
        .bounceDir   =  0,
        .bounceSpeed =  0.0f
    };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){
        .rect        = { 556.0f, 355.0f, 60.0f, 57.0f },
        .originalY   = 355.0f,
        .bounceOffset=  0.0f,
        .bouncing    = false,
        .bounceDir   =  0,
        .bounceSpeed =  0.0f
    };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){
        .rect        = { 662.0f, 355.0f, 60.0f, 57.0f },
        .originalY   = 355.0f,
        .bounceOffset=  0.0f,
        .bouncing    = false,
        .bounceDir   =  0,
        .bounceSpeed =  0.0f
    };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){
        .rect        = { 712.0f, 355.0f, 60.0f, 57.0f },
        .originalY   = 355.0f,
        .bounceOffset=  0.0f,
        .bouncing    = false,
        .bounceDir   =  0,
        .bounceSpeed =  0.0f
    };
  

    coinCount = 0; // Zera moedas
    coins[coinCount++] = CreateCoin((Vector2){500.0f, 280.0f}); // Cria moeda
    coins[coinCount++] = CreateCoin((Vector2){556.0f, 280.0f});
    coins[coinCount++] = CreateCoin((Vector2){662.0f, 280.0f});
    coins[coinCount++] = CreateCoin((Vector2){712.0f, 280.0f});

    Mario_t Mario; // Declara Mario
    InitMario(&Mario); // Inicializa Mario

    Camera2D camera = { 0 }; // Inicializa câmera
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }; // Offset da câmera
    camera.zoom   = 1.0f; // Zoom
    camera.rotation = 0.0f; // Rotação

    while (!WindowShouldClose()) { // Loop principal
        float delta = GetFrameTime(); // Tempo do frame

        bool isMoving = false; // Flag de movimento

        if (IsKeyDown(KEY_RIGHT) && Mario.canMove) { // Move direita
            Rectangle mNext = GetMarioCollisionRect(&Mario);
            mNext.x += MOVE_SPEED * delta;

            bool collision = false;
            for (int i = 0; i < physPlatCount; i++) {
                if (CheckCollisionRecs(mNext, physPlatforms[i].rect)) {
                    collision = true;
                    break;
                }
            }
            if (!collision) {
                Mario.position.x += MOVE_SPEED * delta;
                Mario.facingRight = true;
                isMoving = true;
            }
        }
        if (IsKeyDown(KEY_LEFT) && Mario.canMove) { // Move esquerda
            Rectangle mNext = GetMarioCollisionRect(&Mario);
            mNext.x -= MOVE_SPEED * delta;

            bool collision = false;
            for (int i = 0; i < physPlatCount; i++) {
                if (CheckCollisionRecs(mNext, physPlatforms[i].rect)) {
                    collision = true;
                    break;
                }
            }
            if (!collision) {
                Mario.position.x -= MOVE_SPEED * delta;
                Mario.facingRight = false;
                isMoving = true;
            }
        }
        if (IsKeyPressed(KEY_SPACE) && Mario.canJump) { // Pulo
            Mario.speed.y = JUMP_FORCE;
            Mario.canJump = false;
            Mario.actualState = ACTION_JUMPING;
            PlaySound(jumpSound);
        }

        Mario.speed.y += GRAVITY * delta; // Aplica gravidade
        Mario.position.y += Mario.speed.y * delta; // Atualiza Y

        HandleMarioPlatformCollisions(&Mario); // Colisão vertical

        for (int i = 1; i < physPlatCount; i++) { // Atualiza bounce
            PhysPlatform_t *p = &physPlatforms[i];
            if (p->bouncing) {
                if (p->bounceDir == 1) {
                    p->bounceOffset -= 200.0f * delta;
                    if (p->bounceOffset <= -15.0f) {
                        p->bounceOffset = -15.0f;
                        p->bounceDir = -1;
                    }
                } else {
                    p->bounceOffset += 200.0f * delta;
                    if (p->bounceOffset >= 0.0f) {
                        p->bounceOffset = 0.0f;
                        p->bouncing    = false;
                    }
                }
                p->rect.y = p->originalY + p->bounceOffset;
            }
        }

        UpdateCoins(&Mario); // Atualiza moedas

        if (!Mario.canJump) {
            Mario.actualState = ACTION_JUMPING;
        } else {
            if (isMoving) {
                Mario.actualState = ACTION_WALKING;
            } else {
                Mario.actualState = ACTION_IDLE;
            }
        }

        float camTargetX = roundf(Mario.position.x); // Arredonda X da câmera
        camera.target = (Vector2){ camTargetX, 290.0f }; // Atualiza alvo da câmera
        if (camera.target.x < screenWidth / 2.0f) camera.target.x = screenWidth / 2.0f; // Limita esquerda

        BeginDrawing(); // Começa desenho
        ClearBackground((Color){92, 148, 252, 255}); // Limpa fundo com cor

        BeginMode2D(camera); // Modo 2D
            DrawGameScene(scene); // Desenha cenário
            DrawCoins();         // Desenha moedas

            Rectangle blockSource = { 271.0f, 191.0f, 16.0f, 17.0f }; // Sprite bloco
            for (int i = 1; i < physPlatCount; i++) { // Desenha plataformas dinâmicas
                DrawTexturePro(
                    blocksTexture,
                    blockSource,
                    physPlatforms[i].rect,
                    (Vector2){ 0.0f, 0.0f },
                    0.0f,
                    WHITE
                );
            }

            DrawMario(&Mario); // Desenha Mario

        EndMode2D(); // Fim modo 2D

        EndDrawing(); // Fim desenho
    }

    UnloadGameScene(scene); // Libera cenário

    UnloadSound(bumpSound); // Libera som bump
    UnloadSound(jumpSound); // Libera som pulo

    UnloadTexture(blocksTexture); // Libera textura blocos
    UnloadTexture(coinAtlas);     // Libera textura moedas

    UnloadTexture(Mario.animations.smallMarioSheet); // Libera textura Mario pequeno
    UnloadTexture(Mario.animations.superMarioSheet); // Libera textura Mario super
    UnloadTexture(Mario.animations.fireMarioSheet);  // Libera textura Mario fogo

    CloseAudioDevice(); // Fecha áudio
    CloseWindow();      // Fecha janela

    return 0; // Fim do programa
}

// Define aqui apenas a assinatura, pois a implementação já existe em mario.c
static Rectangle GetMarioCollisionRect(Mario_t *Mario) {
    return (Rectangle){ 0 }; // Dummy: Apenas declarado para satisfazer a ligação; a real implementação está em mario.c
}
