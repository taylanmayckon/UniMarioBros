#ifndef SCENE_H
#define SCENE_H

#include "raylib.h"

// Struct que representa uma plataforma no jogo
typedef struct {
    Rectangle rect;  // Retângulo de colisão da plataforma
} Platform;

// Struct que representa o cenário do jogo
typedef struct {
    Texture2D background;   // Textura do fundo do cenário
    Platform *platforms;    // Endereço de uma variável do tipo platform, para alocação dinâmica de memória
    int platformCount;      // Quantidade de plataformas no cenário
    float scale;            // Escala para ajustar ao tamanho da tela
    float scaledBackgroundWidth;    // Largura do background após escalonamento
} GameScene;

// Protótipos de funções:
GameScene CreateGameScene(const char *backgroundImagePath); // Cria e inicializa o cenário do jogo
void DrawGameScene(GameScene scene); // Desenha todos os elementos do cenário
void UnloadGameScene(GameScene scene); // Libera os recursos alocados pelo cenário

#endif
