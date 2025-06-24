#include <math.h>
#include "raylib.h"
#include "mario.h"
#include "platform.h"
#include "inimigos.h"

void CheckMarioHitboxY(Mario_t *Mario, PhysPlatform_t *physPlatforms, bool *isOnGround, Sound bumpSound){
    for (int i = 0; i < physPlatCount; i++) {
        PhysPlatform_t *p = &physPlatforms[i];
        Rectangle pRect = p->rect;

        if (CheckCollisionRecs(Mario->hitbox, pRect)) {
            // Quando aterrissa no bloco
            if (Mario->speed.y >= 0) {
                Mario->position.y = pRect.y; // Coloca Mario exatamente em cima
                Mario->speed.y = 0;
                *isOnGround = true;
                Mario->canJump = true;

                if (Mario->actualState == ACTION_JUMPING) {
                    Mario->actualState = (fabsf(Mario->speed.x) > 0) ? ACTION_WALKING : ACTION_IDLE;
                }
            } 
            // Detectar a batida de cabeca no bloco
            else { // Mario pulando (speed.y<0)
                float overlap = (pRect.y + pRect.height) - Mario->hitbox.y;
                Mario->position.y += overlap;
                Mario->speed.y = 0;

                if (i > 0 && !p->bouncing) {
                    p->bouncing = true; p->bounceDir = 1; p->bounceOffset = 0.0f;
                    PlaySound(bumpSound);
                }
            }
            MarioHitbox(Mario); // Atualiza novamente a hibox
        }
    }
}


void CheckMarioHitboxX(Mario_t *Mario, PhysPlatform_t *physPlatforms){
    for (int i = 0; i < physPlatCount; i++) {
        Rectangle pRect = physPlatforms[i].rect;
        if (CheckCollisionRecs(Mario->hitbox, pRect)) {
            // Verifica se é uma colisão horizontal, checando a posição do frame anterior
            float prev_hitbox_right = Mario->hitbox.x + Mario->hitbox.width - Mario->speed.x * GetFrameTime();
            float prev_hitbox_left = Mario->hitbox.x - Mario->speed.x * GetFrameTime();

            // Colidindo à direita
            if (Mario->speed.x > 0 && prev_hitbox_right <= pRect.x) {
                Mario->position.x -= (Mario->hitbox.x + Mario->hitbox.width) - pRect.x;
                Mario->speed.x = 0;
            }
            // Colidindo à esquerda
            else if (Mario->speed.x < 0 && prev_hitbox_left >= (pRect.x + pRect.width)) {
                 Mario->position.x += (pRect.x + pRect.width) - Mario->hitbox.x;
                 Mario->speed.x = 0;
            }
            
            MarioHitbox(Mario);
        }
    }
}


// Funçao da colisao Mario-inimigo
void CheckEnemyCollision(Mario_t *Mario, Rectangle enemyRect){
    if(CheckCollisionRecs(Mario->hitbox, enemyRect)){
        Mario->actualState = ACTION_DYING;
    }
}

//  Checa se chegou no chao da bandeira
void CheckFlagEndCollision(Mario_t *Mario, PhysPlatform_t *physPlatforms){
    Mario->position.y += 8.0f;
    for (int i = 0; i < physPlatCount; i++) {
        PhysPlatform_t *p = &physPlatforms[i];
        Rectangle pRect = p->rect;

        if (CheckCollisionRecs(Mario->hitbox, pRect)) {
            // Quando aterrissa no bloco
            Mario->position.y = pRect.y; // Coloca Mario exatamente em cima
        }
        MarioHitbox(Mario); // Atualiza novamente a hibox
    }
}