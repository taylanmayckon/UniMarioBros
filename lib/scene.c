#include "scene.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

// Função para criar e inicializar o cenário
GameScene CreateGameScene(const char *backgroundImagePath) {

    GameScene scene = {0}; // Inicializa a estrutura zerada

    scene.background = LoadTexture(backgroundImagePath); // Carrega textura do fundo
    scene.scale = (float)GetScreenHeight() / (scene.background.height/2); // Calcula escala
    scene.scaledBackgroundWidth = scene.background.width * scene.scale; // Calcula largura escalada

    return scene; // Retorna cenário criado
}

// Função para desenhar o cenário
void DrawGameScene(GameScene scene, bool section) {
    Rectangle sourceRec = { 0, 0, scene.background.width, scene.background.height/2 }; // Retângulo de origem do fundo
    Rectangle destRec = { 0, 0, scene.scaledBackgroundWidth, (float)GetScreenHeight() }; // Retângulo de destino
    
    //Verificação de qual parte do cenário deve ser desenhada
    if (section != false) {
            // Retângulo de origem do fundo para a parte de cima do mapa
            sourceRec.y = 0;
    }
    else {
            // Retângulo de origem do fundo para o túnel
            sourceRec.y = 307;
    }

    DrawTexturePro(
        scene.background,  // Textura do fundo
        sourceRec,         // Região de origem
        destRec,           // Região de destino
        (Vector2){0,0},    // Origem
        0.0f,              // Rotação
        WHITE              // Cor
    );
}

// Função para liberar recursos do cenário
void UnloadGameScene(GameScene scene) {
    UnloadTexture(scene.background); // Libera textura do fundo
    free(scene.platforms);           // Libera vetor de plataformas
}
