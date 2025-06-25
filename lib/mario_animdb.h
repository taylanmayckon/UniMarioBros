#ifndef MARIO_ANIMDB_H
#define MARIO_ANIMDB_H

#include "raylib.h"

// Intervalo de frames quando é loop
typedef struct{
    int start;
    int end;
} FrameRange_t;

// Struct para guardar informações de cada animação
typedef struct{
    bool isLooping; // Se é animação ou frame congelado
    FrameRange_t leftAnimFrames; // Range da animação para direita
    FrameRange_t rightAnimFrames; // Range da animação para esquerda
    int freezedFrameLeft; // Frame único se tiver virado para esquerda (frame congelado)
    int freezedFrameRight; // Frame único se tiver virado para direita (frame congelado)
} AnimData_t;

// "Banco de dados" das animações do Mario
typedef struct{
    AnimData_t idle; // Parado
    AnimData_t walking; // Andando
    AnimData_t jumping; // Pulando
    AnimData_t slide; // Deslizando
    AnimData_t crouch; // Abaixado
    AnimData_t death; // Morte
    AnimData_t throw; // Jogar bola de fogo
    AnimData_t flag; // Descer a bandeira
    AnimData_t pipe; // Descer no cano
} MarioAnimDB_t;

// Prototipo de funções
MarioAnimDB_t InitSmallMarioDB();
MarioAnimDB_t InitSuperMarioDB();
MarioAnimDB_t InitFireMarioDB();

#endif
