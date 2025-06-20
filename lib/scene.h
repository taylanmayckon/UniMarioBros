#ifndef SCENE_H
#define SCENE_H

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
GameScene CreateGameScene(const char *backgroundImagePath);
void DrawGameScene(GameScene scene);
void UnloadGameScene(GameScene scene);

#endif // GAME_SCENE_H
