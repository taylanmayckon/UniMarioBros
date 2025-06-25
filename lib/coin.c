#include <stdio.h>
#include "raylib.h"
#include "coin.h"

// Função para criar uma moeda
Coin_t CreateCoin(Vector2 position) {
    return (Coin_t){
        .position     = position, // Posição
        .frameCounter = 0,        // Contador
        .currentFrame = 0,        // Frame inicial
        .totalFrames  = 4,        // Total de frames
        .frameSpeed   = 8,        // Velocidade
        .frameWidth   = 16.0f,    // Largura
        .frameHeight  = 16.0f,    // Altura
        .sourceRec    = { 0.0f, 0.0f, 16.0f, 16.0f }, // Origem
        .active       = true      // Ativa
    };
}

void InitCoins(Coin_t *coins){
    coinCount = 0; // Zera moedas
    coins[coinCount++] = CreateCoin((Vector2){585.0f, 155.0f}); // Cria moeda
    coins[coinCount++] = CreateCoin((Vector2){705.0f, 0.0f});
    coins[coinCount++] = CreateCoin((Vector2){825.0f, 155.0f});
    coins[coinCount++] = CreateCoin((Vector2){712.0f, 225.0f});
}

// Atualiza moedas
void UpdateCoins(Mario_t *Mario) {
    for (int i = 0; i < coinCount; i++) {
        Coin_t *c = &coins[i]; // Moeda
        if (!c->active) continue; // Se não ativa, pula

        Rectangle coinRect = {
            c->position.x + 3.0f,
            c->position.y + 3.0f,
            c->frameWidth * 3.0f - 6.0f,
            c->frameHeight * 3.0f - 6.0f
        };

        if (CheckCollisionRecs(Mario->hitbox, coinRect)) { // Se colidiu
            c->active = false; // Desativa moeda
            Mario->stats.coins++;    // Incrementa moedas
            Mario->stats.score += 100; // Pontuação
        }

        c->frameCounter++; // Incrementa contador
        if (c->frameCounter >= (int)(60 / c->frameSpeed)) {
            c->frameCounter = 0; // Zera contador
            c->currentFrame = (c->currentFrame + 1) % c->totalFrames; // Próximo frame
            c->sourceRec.x = c->currentFrame * c->frameWidth; // Atualiza origem
        }
    }
}

// Desenha moedas
void DrawCoins(void) {
    for (int i = 0; i < coinCount; i++) {
        Coin_t *c = &coins[i]; // Moeda
        if (!c->active) continue; // Se não ativa, pula

        DrawTexturePro(
            coinAtlas, // Textura moeda
            c->sourceRec, // Origem
            (Rectangle){ c->position.x, c->position.y,
                         c->frameWidth  * 3.0f,
                         c->frameHeight * 3.0f },
            (Vector2){ 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }
}

// Variáveis definidas em coin.c
int coinCount = 0;
Coin_t coins[MAX_COINS];
Texture2D coinAtlas;
