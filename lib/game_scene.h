#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "raylib.h"

// Estrutura de uma plataforma fixa
typedef struct {
    Rectangle rect;  // Retângulo da plataforma
} Platform;

// Estrutura do cenário do jogo
typedef struct {
    Texture2D background;         // Textura do fundo
    Platform *platforms;          // Vetor de plataformas
    int       platformCount;      // Quantidade de plataformas
    float     scale;              // Fator de escala do fundo
    float     scaledBackgroundWidth; // Largura do fundo escalado
} GameScene;

// Prototipação das funções
static GameScene CreateGameScene(const char *backgroundImagePath);
static void DrawGameScene(GameScene scene);
static void UnloadGameScene(GameScene scene);

#endif // GAME_SCENE_H
