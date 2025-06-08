#include <stdbool.h>
#include "math.h"
#include "raylib.h"
#include "platform.h"

// Definições das constantes
const float GRAVITY     = 1800.0f;
const float MOVE_SPEED  = 300.0f;
const float JUMP_FORCE  = -870.0f;

// Contador de plataformas dinâmicas
int physPlatCount = 0;

void InitPlatforms(PhysPlatform_t *physPlatforms){
    physPlatCount = 0;

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
}

void UpdatePlatforms(PhysPlatform_t *physPlatform){

    for (int i = 1; i < physPlatCount; i++) { // Atualiza bounce
        if (physPlatform[i].bouncing) {
            if (physPlatform[i].bounceDir == 1) {
                physPlatform[i].bounceOffset -= 200.0f * GetFrameTime();
                if (physPlatform[i].bounceOffset <= -15.0f) {
                    physPlatform[i].bounceOffset = -15.0f;
                    physPlatform[i].bounceDir = -1;
                }
            } else {
                physPlatform[i].bounceOffset += 200.0f * GetFrameTime();
                if (physPlatform[i].bounceOffset >= 0.0f) {
                    physPlatform[i].bounceOffset = 0.0f;
                    physPlatform[i].bouncing = false;
                }
            }
            physPlatform[i].rect.y = physPlatform[i].originalY + physPlatform[i].bounceOffset;
        }
    }
}


void DrawBlocks(PhysPlatform_t *physPlatforms, Texture2D block1){
    Rectangle blockSource = { 271.0f, 191.0f, 16.0f, 17.0f }; // Sprite bloco
    for (int i = 1; i < physPlatCount; i++) { // Desenha plataformas dinâmicas
        // Forma original que tava antes
        // DrawTexturePro(
        //     LoadTexture("assets/textures/ItemsAndBlock.png"),
        //     blockSource,
        //     physPlatforms[i].rect,
        //     (Vector2){ 0.0f, 0.0f },
        //     0.0f,
        //     WHITE
        // );

        // Forma para testes com block1 do github
        DrawTexturePro(
            block1,
            (Rectangle){16.0f, 16.0f, 16.0f, 16.0f},
            physPlatforms[i].rect,
            (Vector2){ 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }
}

// Trata colisão Mario/plataforma
void HandleMarioPlatformCollisions(Mario_t *Mario, PhysPlatform_t *physPlatforms, Sound bumpSound) {
    Rectangle mRect = GetMarioCollisionRect(Mario); // Retângulo do Mario

    for (int i = 0; i < physPlatCount; i++) {
        PhysPlatform_t *p = &physPlatforms[i]; // Plataforma
        Rectangle pRect = p->rect;             // Retângulo da plataforma

        if (CheckCollisionRecs(mRect, pRect)) { // Se colidiu
            if (Mario->speed.y > 0 && (mRect.y + mRect.height - (Mario->speed.y * GetFrameTime())) <= pRect.y) {
                Mario->position.y = pRect.y; // Coloca Mario em cima
                Mario->speed.y = 0;          // Zera velocidade Y
                Mario->canJump = true;       // Pode pular
            }
            else if (Mario->speed.y < 0 && (mRect.y) <= (pRect.y + pRect.height) &&
                     (mRect.y + mRect.height) > (pRect.y + pRect.height))
            {
                Mario->speed.y = 0; // Zera velocidade Y

                if (i > 0 && !p->bouncing) { // Se não for chão
                    p->bouncing = true;      // Ativa bounce
                    p->bounceDir = 1;        // Sobe
                    p->bounceOffset = 0.0f;  // Zera offset
                    PlaySound(bumpSound);    // Toca som
                }
            }
        }
    }
}