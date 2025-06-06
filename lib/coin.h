#ifndef COIN_H
#define COIN_H

#include "raylib.h"

// Estrutura da moeda animada
typedef struct {
    Vector2   position;    // Posição
    Rectangle sourceRec;   // Retângulo de origem
    int       currentFrame;// Frame atual
    int       frameCounter;// Contador de frames
    int       totalFrames; // Total de frames
    int       frameSpeed;  // Velocidade da animação
    float     frameWidth;  // Largura do frame
    float     frameHeight; // Altura do frame
    bool      active;      // Ativa ou não
} Coin_t;

// Máximo de moedas
#define MAX_COINS 10

// Contador de moedas
extern int  coinCount;
// Vetor de moedas
extern Coin_t coins[MAX_COINS];

// Textura das moedas
extern Texture2D coinAtlas;

// Prototipação de funções auxiliares
static Coin_t CreateCoin(Vector2 position);
static void UpdateCoins(struct Mario_t *Mario);
static void DrawCoins(void);

#endif // COIN_H
