#include <stdio.h>
#include "raylib.h"
#include "mario.h"
#include "mario_animdb.h"


// (NORMAL MARIO) definições para as sprites dele andando (192x18 total -> 16x18/frame)
#define NORMAL_MARIO_FRAME_WIDHT_CUT 16.0f
#define NORMAL_MARIO_FRAME_HEIGHT_CUT 16.0f

// (SUPER MARIO) definições para as sprites dele andando (228x34 total -> 16,0x34/frame)
#define SUPER_MARIO_FRAME_WIDHT_CUT 16.0f
#define SUPER_MARIO_FRAME_HEIGHT_CUT 30.0f

// (FIRE MARIO) placeholders, falta alterar
#define FIRE_MARIO_FRAME_WIDHT_CUT 16.0f
#define FIRE_MARIO_FRAME_HEIGHT_CUT 34.0f

// Escala que vai ser desenhado na tela a sprite
#define MARIO_SPRITE_SCALE 2.0f

// Função para inicializar a sprite do Mario com os parâmetros para o Small Mario parado
void InitSprite(MarioSprite_t *sprite, Texture2D texture, Rectangle original_frame_pos_scale, float frameSpeed, float frameTimer, int currentFrame){
    sprite->spriteSheet = texture; // Carregando a sprite já renderizada
    sprite->sourceRec = original_frame_pos_scale; // Como o sheet será cortado
    sprite->frameSpeed = frameSpeed; // Tempo com que os frames serão alterados
    sprite->frameTimer = frameTimer; // Contador de tempo atual do frame
    sprite->currentFrame = currentFrame; // Frame atual
    sprite->frameWidthCut = original_frame_pos_scale.width;
    sprite->frameHeightCut = original_frame_pos_scale.height;
}

// Função para inicializar a struct Mario
void InitMario(Mario_t *Mario){
    printf("[InitMario] Running\n");

    Mario->position = (Vector2){300.0f, 300.0f}; // Posição inicial
    Mario->speed = (Vector2){0.0f, 0.0f}; // Inicia em repouso (vx, vy = 0)
    Mario->invincible = false; // Inicia "vencível"
    Mario->facingRight = true; // Virado para direita
    Mario->canJump = true; // Consegue pular
    Mario->powerUpState = STATE_SMALL; // Estado do mario (normal, super,)
    Mario->actualState = ACTION_IDLE; // Parado
    Mario->lives=3; // Contador de vidas
    Mario->score=0; // Pontuação
    Mario->coins=0; // Quant. de moedas

    // Renderizando os arquivos das sprites
    Mario->animations.superMarioSheet = LoadTexture("assets/textures/mario/supermario.png");
    Mario->animations.smallMarioSheet = LoadTexture("assets/textures/mario/smallmario.png");

    // Pegando os dados do "Banco de dados" de animações
    Mario->animations.smallMarioAnimDB = InitSmallMarioDB();
    Mario->animations.superMarioAnimDB = InitSuperMarioDB();
    Mario->animations.fireMarioAnimDB = InitFireMarioDB();

    // Iniciando as sprites
    // -> Mario Normal
    InitSprite(
        &Mario->animations.smallMarioSprite, // Ponteiro para a sprite 
        Mario->animations.smallMarioSheet, // Ponteiro para a textura carregada
        (Rectangle){0.0f, 0.0f, // Posição da origem dos frames
                    NORMAL_MARIO_FRAME_WIDHT_CUT, NORMAL_MARIO_FRAME_HEIGHT_CUT}, // Largura e altura de cada frame
        0.1f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        7); // Frame atual
    // -> Super Mario
    InitSprite(
        &Mario->animations.superMarioSprite, // Ponteiro para a sprite 
        Mario->animations.superMarioSheet, // Ponteiro para a textura carregada
        (Rectangle){0.0f, 0.0f, // Posição da origem dos frames
                    SUPER_MARIO_FRAME_WIDHT_CUT, SUPER_MARIO_FRAME_HEIGHT_CUT}, // Largura e altura de cada frame
        0.1f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        6); // Frame atual
    // -> Fire Mario (placeholder atualmente)
    InitSprite(
        &Mario->animations.fireMarioSprite, // Ponteiro para a sprite 
        Mario->animations.fireMarioSheet, // Ponteiro para a textura carregada
        (Rectangle){0.0f, 0.0f, // Posição da origem dos frames
                    FIRE_MARIO_FRAME_WIDHT_CUT, FIRE_MARIO_FRAME_HEIGHT_CUT}, // Largura e altura de cada frame
        0.1f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        6); // Frame atual

    // -> Iniciando com a sprite do mario normal
    Mario->animations.activeSprite = &Mario->animations.smallMarioSprite;
    
    printf("[InitMario] Finished\n");
}

// Função para mover a sprite de lugar e desenhá-la
void ChangeMarioSpritePosition(Mario_t *Mario, float width_scale, float height_scale){
    MarioSprite_t *sprite = Mario->animations.activeSprite;
    
    // Posição
    sprite->destRec.x = Mario->position.x;
    sprite->destRec.y = Mario->position.y;
    // Proporção, tem que ser ajustada corretamente depois
    sprite->destRec.width = width_scale;
    sprite->destRec.height = height_scale;

    // Alterna o frame da sprite com base no contador
    sprite->sourceRec.x = (float)(sprite->currentFrame * sprite->frameWidthCut); // Tem que passar como parâmetro pra função, pode variar de uma sprite para outra
    //printf("[ChangeMarioSpritePosition] SourceRec.x: %.2f\n", sprite->sourceRec.x); // Depuração
    Vector2 origin = {0, 0}; // Ponto de origem para rotação/escala (mantendo só porque, vai que né)
    
    // Finalmente desenhando
    DrawTexturePro(
        sprite->spriteSheet, // A sprite inteira (todos frames)
        sprite->sourceRec, // Como vai cortar a sprite
        sprite->destRec, // Local e tamanho na tela
        origin, // Ponto em que vai rotacionar
        0.0f, // Rotação em graus
        WHITE // Filtro (WHITE = sem alteração)
    );
}

// Função auxiliar para não permitir um index fora do range na troca de animações
void ConstrainIndex(Mario_t *Mario, FrameRange_t range){
    MarioSprite_t *sprite = Mario->animations.activeSprite;

    if(sprite->currentFrame < range.start){
        sprite->currentFrame = range.start;
    }
    else if(sprite->currentFrame > range.end){
        sprite->currentFrame = range.end;
    }
}

// Função para CONTROLAR a temporização dos frames das sprites
void ChangeSpriteTimer(Mario_t *Mario, FrameRange_t range){ // first_frame e end_frame são o intervalo da animação    
    MarioSprite_t *sprite = Mario->animations.activeSprite;

    // Corrigindo o range caso esteja fora do intervalo
    ConstrainIndex(Mario, range);

    // Acumula o tempo que passou desde o ultimo frame
    // Na pratica é um cronometro
    sprite->frameTimer += GetFrameTime();
    if(sprite->frameTimer >= sprite->frameSpeed){ // Verificando se tá na hora de alterar o frame da animação
        sprite->frameTimer = 0; // Reinicia o cronometro

        // Movendo para DIREITA
        if(Mario->facingRight){ 
            sprite->currentFrame++;
            if(sprite->currentFrame>range.end){
                sprite->currentFrame=range.start;
            }
        }
        // Movendo para ESQUERDA
        else{
            sprite->currentFrame--;
            if(sprite->currentFrame<range.start){
                sprite->currentFrame=range.end;
            }
            
        }
        
    }
}

// Função para selecionar qual sprite será manipulada e desenhada
void DrawMario(Mario_t *Mario){
    // Ponteiros para manipular as infos de animação de Mario
    MarioAnimDB_t *currentAnimDB = NULL; 
    AnimData_t *currentAnimData = NULL; 
    
    // Selecionando o banco de dados das animações, conforme o estado atual do Mario e alterando o endereço para a sprite ativa
    switch(Mario->powerUpState){
        case STATE_SMALL:
            currentAnimDB = &Mario->animations.smallMarioAnimDB; // Atualiza o DB
            Mario->animations.activeSprite = &Mario->animations.smallMarioSprite; // Pega o endereço correto para a sprite do estado atual
            break;
        case STATE_SUPER:
            currentAnimDB = &Mario->animations.superMarioAnimDB; // Atualiza o DB
            Mario->animations.activeSprite = &Mario->animations.superMarioSprite; // Pega o endereço correto para a sprite do estado atual
            break;
        case STATE_FIRE:
            currentAnimDB = &Mario->animations.fireMarioAnimDB; // Atualiza o DB
            Mario->animations.activeSprite = &Mario->animations.fireMarioSprite; // Pega o endereço correto para a sprite do estado atual
            break;
    }

    // Selecionando os dados da animação atual
    switch (Mario->actualState) {
        case ACTION_IDLE:
            currentAnimData = &currentAnimDB->idle;
            break;
        case ACTION_WALKING:
            currentAnimData = &currentAnimDB->walking;
            break;
        case ACTION_JUMPING:
            currentAnimData = &currentAnimDB->jumping;
            break;
        case ACTION_SLIDE:
            currentAnimData = &currentAnimDB->slide;
            break;
        case ACTION_CROUCH:
            currentAnimData = &currentAnimDB->crouch;
            break;
        case ACTION_DEATH:
            currentAnimData = &currentAnimDB->death;
            break;
    }

    // Usando os dados selecionados para animar e desenhar
    if(currentAnimData->isLooping){ // Se é animação com mais de 1 frame
        FrameRange_t range;

        // Pegando o range da animação
        if(Mario->facingRight){ // Virado para direita
            range = currentAnimData->rightAnimFrames;
        }
        else{ // Virado para esquerda
            range = currentAnimData->leftAnimFrames;
        }
        ChangeSpriteTimer(Mario, range); // Função que verifica o momento de alterar o frame, e o faz quando chega
    }
    else{ // Se é frame fixo
        if(Mario->facingRight){ // Virado para direita
            Mario->animations.activeSprite->currentFrame = currentAnimData->freezedFrameRight;
        }
        else{ // Esquerda
            Mario->animations.activeSprite->currentFrame = currentAnimData->freezedFrameLeft;
        }
    }

    // Desenha a sprite na tela
    ChangeMarioSpritePosition(
        Mario,
        Mario->animations.activeSprite->frameWidthCut * MARIO_SPRITE_SCALE,
        Mario->animations.activeSprite->frameHeightCut * MARIO_SPRITE_SCALE);
}

