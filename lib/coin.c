#include <stdio.h>
#include "raylib.h"
#include "coin.h"

// === VARIÁVEIS GLOBAIS ===
int coinCount = 0;
Coin_t coins[MAX_COINS];
Texture2D coinAtlas;

FloatingScore_t floatingScores[MAX_FLOATING_SCORES]; // Para textos +100

// === MOEDA ===
Coin_t CreateCoin(Vector2 position) {
    return (Coin_t){
        .position     = position,
        .frameCounter = 0,
        .currentFrame = 0,
        .totalFrames  = 4,
        .frameSpeed   = 8,
        .frameWidth   = 16.0f,
        .frameHeight  = 16.0f,
        .sourceRec    = { 0.0f, 0.0f, 16.0f, 16.0f },
        .active       = true
    };
}

void InitCoins(Coin_t *coinsArray) {
    coinCount = 0;
    coins[coinCount++] = CreateCoin((Vector2){585.0f, 125.0f});
    coins[coinCount++] = CreateCoin((Vector2){705.0f, 0.0f});
    coins[coinCount++] = CreateCoin((Vector2){825.0f, 155.0f});

    float startX = 1252.0f;
    for (int i = 0; i < 3; i++) {
        coins[coinCount++] = CreateCoin((Vector2){startX + i * 60.0f, 125.0f});
    }

    startX = 2006.0f;
    for (int i = 0; i < 5; i++) {
        coins[coinCount++] = CreateCoin((Vector2){startX + i * 60.0f, 125.0f});
    }

    startX = 2507.0f;
    for (int i = 0; i < 5; i++) {
        coins[coinCount++] = CreateCoin((Vector2){startX + i * 60.0f, 125.0f});
    }
}

void UpdateCoins(Mario_t *Mario) {
    for (int i = 0; i < coinCount; i++) {
        Coin_t *c = &coins[i];
        if (!c->active) continue;

        Rectangle coinRect = {
            c->position.x + 3.0f,
            c->position.y + 3.0f,
            c->frameWidth * 3.0f - 6.0f,
            c->frameHeight * 3.0f - 6.0f
        };

        if (CheckCollisionRecs(Mario->hitbox, coinRect)) {
            c->active = false;
            Mario->stats.coins++;
            Mario->stats.score += 100;

            // Mostra pontuação na posição da moeda
            AddScoreAtPosition((Vector2){c->position.x, c->position.y - 10.0f}, 100);
        }

        c->frameCounter++;
        if (c->frameCounter >= (int)(60 / c->frameSpeed)) {
            c->frameCounter = 0;
            c->currentFrame = (c->currentFrame + 1) % c->totalFrames;
            c->sourceRec.x = c->currentFrame * c->frameWidth;
        }
    }
}

void DrawCoins(void) {
    for (int i = 0; i < coinCount; i++) {
        Coin_t *c = &coins[i];
        if (!c->active) continue;

        DrawTexturePro(
            coinAtlas,
            c->sourceRec,
            (Rectangle){ c->position.x, c->position.y, c->frameWidth * 3.0f, c->frameHeight * 3.0f },
            (Vector2){ 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }
}

// === PONTUAÇÃO FLUTUANTE ===
void AddScoreAtPosition(Vector2 position, int value) {
    for (int i = 0; i < MAX_FLOATING_SCORES; i++) {
        if (!floatingScores[i].active) {
            floatingScores[i] = (FloatingScore_t){
                .position = position,
                .value = value,
                .alpha = 1.0f,
                .lifetime = 1.0f, // 1 segundo visível
                .active = true
            };
            break;
        }
    }
}

void UpdateFloatingScores(void) {
    float dt = GetFrameTime();
    for (int i = 0; i < MAX_FLOATING_SCORES; i++) {
        if (!floatingScores[i].active) continue;

        floatingScores[i].lifetime -= dt;
        floatingScores[i].position.y -= 30.0f * dt;  // Sobe
        floatingScores[i].alpha -= dt;              // Fica transparente

        if (floatingScores[i].lifetime <= 0 || floatingScores[i].alpha <= 0) {
            floatingScores[i].active = false;
        }
    }
}

void DrawFloatingScores(void) {
    for (int i = 0; i < MAX_FLOATING_SCORES; i++) {
        if (!floatingScores[i].active) continue;

        Color color = Fade(YELLOW, floatingScores[i].alpha);
        char text[16];
        snprintf(text, sizeof(text), "+%d", floatingScores[i].value);
        DrawText(text, floatingScores[i].position.x, floatingScores[i].position.y, 20, color);
    }
}
