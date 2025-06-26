#ifndef COIN_H
#define COIN_H

#include "raylib.h"
#include "mario.h"

// Estrutura da moeda animada
typedef struct {
    Vector2   position;     // Posição da moeda
    Rectangle sourceRec;    // Retângulo da textura
    int       currentFrame; // Frame atual da animação
    int       frameCounter; // Contador de tempo da animação
    int       totalFrames;  // Total de frames
    int       frameSpeed;   // Velocidade de troca de frame
    float     frameWidth;   // Largura do frame
    float     frameHeight;  // Altura do frame
    bool      active;       // Se está visível ou já foi coletada
} Coin_t;

// --- Pontuação flutuante ao coletar moeda ou matar inimigo ---
typedef struct {
    Vector2 position;       // Posição onde o texto aparece
    int     value;          // Pontuação (ex: 100)
    float   alpha;          // Transparência do texto
    float   lifetime;       // Tempo restante visível
    bool    active;         // Se está visível ou não
} FloatingScore_t;

#define MAX_COINS 100
#define MAX_FLOATING_SCORES 32

// Moedas
extern int coinCount;
extern Coin_t coins[MAX_COINS];
extern Texture2D coinAtlas;

// Animações de pontuação
extern FloatingScore_t floatingScores[MAX_FLOATING_SCORES];

// Funções de moedas
Coin_t CreateCoin(Vector2 position);
void InitCoins(Coin_t *coins);
void UpdateCoins(Mario_t *Mario);
void DrawCoins(void);

// Funções da animação de pontuação
void AddScoreAtPosition(Vector2 position, int value);
void UpdateFloatingScores(void);
void DrawFloatingScores(void);

#endif // COIN_H
