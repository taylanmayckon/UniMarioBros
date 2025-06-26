#ifndef SCENE_H
#define SCENE_H

#include "raylib.h"
#include <stdbool.h>

// Estrutura do cenário do jogo
typedef struct {
    Texture2D background;         // Textura do fundo
    float     scale;              // Fator de escala do fundo
    float     scaledBackgroundWidth; // Largura do fundo escalado
} GameScene;

// Prototipação das funções
GameScene CreateGameScene(const char *backgroundImagePath);
void DrawGameScene(GameScene scene, bool section);
void UnloadGameScene(GameScene scene);

#endif
