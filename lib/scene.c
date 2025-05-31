#include "scene.h"
#include <stdlib.h>

// Implementação da função CreateGameScene, que recebe como parâmetro o endereço da imagem de background
GameScene CreateGameScene(const char *backgroundImagePath) {

    GameScene scene = {0}; // Declara a estrutura do cenário vazia
    
    scene.background = LoadTexture(backgroundImagePath); // Carrega a textura do background
    scene.scale = (float)GetScreenHeight() / (scene.background.height/2); // Calcula o fator de escala entre o background e a tela
    scene.scaledBackgroundWidth = scene.background.width * scene.scale; // Calcula a largura do background após o escalonamento
    
    // Define as plataformas para o chão com suas coordenadas originais (em pixels da imagem) em formato {x, y, largura, altura}
    Rectangle platformRects[] = {
        {0, 226, 514, 80},    // Plataforma 1
        {575, 226, 207, 80},   // Plataforma 2
        {864, 226, 367, 80},   // Plataforma 3
        {1294, 226, 613, 80}   // Plataforma 4
    };
    
    scene.platformCount = sizeof(platformRects)/sizeof(Rectangle); // Calcula o número de plataformas
    
    // Aloca memória dinamicamente para o vetor de plataformas
    scene.platforms = (Platform *)malloc(scene.platformCount * sizeof(Platform));
    
    // Converte as coordenadas das plataformas para a escala da tela
    for (int i = 0; i < scene.platformCount; i++) {
        
        scene.platforms[i].rect.x = platformRects[i].x * scene.scale;
        scene.platforms[i].rect.y = platformRects[i].y * scene.scale;
        scene.platforms[i].rect.width = platformRects[i].width * scene.scale;
        scene.platforms[i].rect.height = platformRects[i].height * scene.scale;
    }
    
    return scene; // Retorna o struct do cenário configurada
}

// Implementação da função DrawGameScene
void DrawGameScene(GameScene scene) {

    Rectangle sourceRec = { 0, 0, scene.background.width, scene.background.height/2 }; // Define o retângulo de origem
    Rectangle destRec = { 0, 0, scene.scaledBackgroundWidth, (float)GetScreenHeight() }; // Define o retângulo de destino
    
    // Desenha o background na tela
    DrawTexturePro(
        scene.background,  // Textura a ser desenhada
        sourceRec,         // Região de origem na textura
        destRec,          // Região de destino na tela
        (Vector2){0,0},   // Ponto de origem (rotação/escala)
        0.0f,            // Ângulo de rotação
        WHITE            // Cor de tintura (sem alteração de cor)
    );
}

// Implementação da função UnloadGameScene
void UnloadGameScene(GameScene scene) {

    UnloadTexture(scene.background); // Libera a textura do background
    free(scene.platforms); // Libera o vetor de plataformas alocado dinamicamente

}
