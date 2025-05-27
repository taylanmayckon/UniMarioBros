#include "raylib.h"
#include "mario.h"

// Inicializando Mario_t
Mario_t Mario;

void DrawMario(){
    // Testando imprimir frames de andando para direita

    // Definições para a sprite walk right (174x16 total -> 29x16/frame)
    const float WALK_RIGHT_FRAME_WIDHT = 29.0f;
    const float WALK_RIGHT_FRAME_HEIGHT = 16.0f;

    Mario.walkRight.spriteSheet = LoadTexture("assets/textures/mario/mario1-walkright.png");
    // Selecionando qual frame será exibido
    Mario.walkRight.sourceRec.x = 0.0f; // X de origem
    Mario.walkRight.sourceRec.y = 0.0f; // Y de origem
    Mario.walkRight.sourceRec.width = WALK_RIGHT_FRAME_WIDHT; // X do corte
    Mario.walkRight.sourceRec.height = WALK_RIGHT_FRAME_HEIGHT; // Y do corte

    // Frame atual
    Mario.walkRight.currentFrame = 0;

    // Selecionando posição e escala do desenho que será feito no jogo
    Mario.walkRight.destRec.x = 300.0f;
    Mario.walkRight.destRec.y = 300.0f;
    Mario.walkRight.destRec.width = WALK_RIGHT_FRAME_WIDHT*4;
    Mario.walkRight.destRec.height = WALK_RIGHT_FRAME_HEIGHT*4;

    Vector2 origin = {0, 0}; // Ponto de origem para rotação/escala

    DrawTexturePro(
        Mario.walkRight.spriteSheet, // A sprite inteira (todos frames)
        Mario.walkRight.sourceRec, // Como vai cortar a sprite
        Mario.walkRight.destRec, // Local e tamanho na tela
        origin, // Ponto em que vai rotacionar
        0.0f, // Rotação em graus
        WHITE // Filtro (WHITE = sem alteração)
    );
   
}

