#include <stdbool.h>
#include "raylib.h"
#include "platform.h"

// Definições das constantes
const float GRAVITY     = 1800.0f;
const float MOVE_SPEED  = 300.0f;
const float JUMP_FORCE  = -870.0f;

// Contador de plataformas dinâmicas
int physPlatCount = 0;
// Vetor de plataformas dinâmicas
PhysPlatform_t physPlatforms[MAX_PHYS_PLATFORMS];

// Trata colisão Mario/plataforma
void HandleMarioPlatformCollisions(Mario_t *Mario) {
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