#include "raylib.h"
#include "mario.h"
#include <stdio.h>

// Definições para a sprite walk right (174x16 total -> 29x16/frame)
#define WALK_RIGHT_FRAME_WIDHT 29.0f
#define WALK_RIGHT_FRAME_HEIGHT 16.0f

void InitMario(Mario_t *Mario){
    // Função genérica para inicializar a estrutura de uma sprite/animação

    // Testando imprimir frames de andando para direita
    Mario->walkRight.spriteSheet = LoadTexture("assets/textures/mario/mario1-walkright.png");
    // Selecionando qual frame será exibido
    Mario->walkRight.sourceRec.x = 0.0f; // X de origem DO FRAME
    Mario->walkRight.sourceRec.y = 0.0f; // Y de origem DO FRAME

    Mario->position.x = 300.0f;
    Mario->position.y = 300.0f;

    Mario->walkRight.sourceRec.width = WALK_RIGHT_FRAME_WIDHT; // X do corte
    Mario->walkRight.sourceRec.height = WALK_RIGHT_FRAME_HEIGHT; // Y do corte

    // Velocidade do frame
    Mario->walkRight.frameSpeed = 0.2f; // 1s entre troca de frames
    // Quantidade de frames TOTAL na sprite
    Mario->walkRight.frameCount = 6;
    // Contador do frame atual
    Mario->walkRight.frameTimer = 0.0f;

    // Frame atual
    Mario->walkRight.currentFrame = 0;
}

void UpdateMario(Mario_t *Mario){
    // Função para temporizar a alteração de frames do Mario

    
    
    // Acumula o tempo que passou desde o ultimo frame
    // Na pratica é um cronometro
    Mario->walkRight.frameTimer += GetFrameTime();
    // Depuração
    printf("[UpdateMario] Timer: %.2f, Frame Atual:%d\n", Mario->walkRight.frameTimer, Mario->walkRight.currentFrame);
    printf("[UpdateMario] GetFrameTime(): %f\n", GetFrameTime());

    // Verificando se tá na hora de alterar o frame da animação
    if(Mario->walkRight.frameTimer >= Mario->walkRight.frameSpeed){
        Mario->walkRight.frameTimer = 0; // Reinicia o cronometro
        Mario->walkRight.currentFrame++; // Avança para o próximo frmae
        
        // Restringindo as animações para valores permitidos (volta ao inicio quando estoura o max)
        if(Mario->walkRight.currentFrame >= Mario->walkRight.frameCount){
            Mario->walkRight.currentFrame = 0;
        }
    }
}

void DrawMario(Mario_t *Mario){

    // Selecionando posição que será desenhado o Mario na tela
    Mario->walkRight.destRec.x = Mario->position.x;
    Mario->walkRight.destRec.y = Mario->position.y;
    // Proporção do Mario
    Mario->walkRight.destRec.width = WALK_RIGHT_FRAME_WIDHT*4;
    Mario->walkRight.destRec.height = WALK_RIGHT_FRAME_HEIGHT*4;

    // Alterna o frame da sprite com base no contador
    Mario->walkRight.sourceRec.x = (float)Mario->walkRight.currentFrame * WALK_RIGHT_FRAME_WIDHT;

    Vector2 origin = {0, 0}; // Ponto de origem para rotação/escala

    DrawTexturePro(
        Mario->walkRight.spriteSheet, // A sprite inteira (todos frames)
        Mario->walkRight.sourceRec, // Como vai cortar a sprite
        Mario->walkRight.destRec, // Local e tamanho na tela
        origin, // Ponto em que vai rotacionar
        0.0f, // Rotação em graus
        WHITE // Filtro (WHITE = sem alteração)
    );
   
}

