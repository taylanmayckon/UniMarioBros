#include <stdio.h>
#include "raylib.h"
#include "mario.h"
#include "mario_animdb.h"


// (NORMAL MARIO) definições para as sprites dele andando (190x18 total -> 19,0x18/frame)
#define NORMAL_MARIO_FRAME_WIDHT_CUT 19.0f
#define NORMAL_MARIO_FRAME_HEIGHT_CUT 18.0f

// (SUPER MARIO) definições para as sprites dele andando (228x34 total -> 19,0x34/frame)
#define SUPER_MARIO_FRAME_WIDHT_CUT 19.0f
#define SUPER_MARIO_FRAME_HEIGHT_CUT 34.0f

// (FIRE MARIO) placeholders, falta alterar
#define FIRE_MARIO_FRAME_WIDHT_CUT 19.0f
#define FIRE_MARIO_FRAME_HEIGHT_CUT 34.0f

// Escala que vai ser desenhado na tela a sprite
#define MARIO_SPRITE_SCALE 2.0f

// Função para inicializar as sprites
void InitSprite(MarioSprite_t *sprite, Texture2D *texture, Rectangle original_frame_pos_scale, float frameSpeed, float frameTimer, int currentFrame){
    sprite->spriteSheet = *texture; // Carregando a sprite já renderizada
    sprite->sourceRec = original_frame_pos_scale; // Como o sheet será cortado
    sprite->frameSpeed = frameSpeed; // Tempo com que os frames serão alterados
    sprite->frameTimer = frameTimer; // Contador de tempo atual do frame
    sprite->currentFrame = currentFrame; // Frame atual
    sprite->revertAnim = false; // Inicia com a animação no sentido normal
}

// Função para inicializar a struct Mario
void InitMario(Mario_t *Mario){
    printf("[InitMario] Running\n");

    Mario->position = (Vector2){300.0f, 300.0f}; // Posição inicial
    Mario->speed = (Vector2){0.0f, 0.0f}; // Inicia em repouso (vx, vy = 0)
    Mario->invincible = false; // Inicia "vencível"
    Mario->facingRight = false; // Virado para direita
    Mario->canJump = true; // Consegue pular
    Mario->powerUpState = STATE_SUPER; // Estado do mario (normal, super,)
    Mario->actualState = ACTION_WALKING; // Parado
    Mario->lives=3; // Contador de vidas
    Mario->score=0; // Pontuação
    Mario->coins=0; // Quant. de moedas

    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Renderizando os arquivos das sprites
    // PS: na notação [x:y], y é inclusivo 
    // -> Super Mario
    Mario->animations.superMarioSheet = LoadTexture("assets/textures/mario/supermario.png");
    // No que foi renderizado contém:
    // [0] Agachado para esquerda
    // [1] Pulando para esquerda
    // [2] Parando de correr, direção esquerda
    // [3:5] Andando para esquerda
    // [5] Parado virado para esquerda
    // [6] Parado virado para direita
    // [6:8] Andando para direita
    // [9] Parando de correr, direção direita
    // [10] Pulando para direita
    // [11] Agachado para direita

    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Pegando os dados do "Banco de dados" de animações
    Mario->animations.smallMarioAnimDB = InitSmallMarioDB();
    Mario->animations.superMarioAnimDB = InitSuperMarioDB();
    Mario->animations.fireMarioAnimDB = InitFireMarioDB();

    InitSprite(
        &Mario->animations.activeSprite, // Ponteiro para a sprite 
        &Mario->animations.superMarioSheet, // Ponteiro para a textura carregada
        (Rectangle){0.0f, 0.0f, // Posição da origem dos frames
                    SUPER_MARIO_FRAME_WIDHT_CUT, SUPER_MARIO_FRAME_HEIGHT_CUT}, // Largura e altura de cada frame
        0.1f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        6); // Frame atual
    
    printf("[InitMario] Finished\n");
}

// Função para mover a sprite de lugar e desenhá-la
void ChangeMarioSpritePosition(Mario_t *Mario, MarioSprite_t *sprite, float width_scale, float height_scale){
    
    // Posição
    sprite->destRec.x = Mario->position.x;
    sprite->destRec.y = Mario->position.y;
    // Proporção, tem que ser ajustada corretamente depois
    sprite->destRec.width = width_scale;
    sprite->destRec.height = height_scale;

    // Alterna o frame da sprite com base no contador
    sprite->sourceRec.x = (float)(sprite->currentFrame* sprite->frameWidthCut); // Tem que passar como parâmetro pra função, pode variar de uma sprite para outra
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

// Função para CONTROLAR a temporização dos frames das sprites
void ChangeSpriteTimer(Mario_t *Mario, MarioSprite_t *sprite, FrameRange_t range){ // first_frame e end_frame são o intervalo da animação    
    // Acumula o tempo que passou desde o ultimo frame
    // Na pratica é um cronometro
    sprite->frameTimer += GetFrameTime();
    if(sprite->frameTimer >= sprite->frameSpeed){ // Verificando se tá na hora de alterar o frame da animação
        sprite->frameTimer = 0; // Reinicia o cronometro

        // Movendo para DIREITA
        if(Mario->facingRight){ 
            // Incrementa (ciclo normal da animação)
            if(!sprite->revertAnim){
                sprite->currentFrame++; // Avança para o próximo frmae
                // Restringindo as animações para valores permitidos
                if(sprite->currentFrame == range.end){ 
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
            // Decrementa (ciclo inverso para suavizar animação)
            else{
                sprite->currentFrame--;
                // Restrigindo as animações para vlaores permitidos
                if(sprite->currentFrame == range.start){
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
        }
        // Movendo para ESQUERDA
        else{
            // Decrementa (ciclo normal da animação)
            if(!sprite->revertAnim){
                sprite->currentFrame--; 
                // Restrigindo as animações para valores permitidos
                if(sprite->currentFrame == range.start){
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
            // Incrementa (ciclo inverso para suavizar animação)
            else{
                sprite->currentFrame++;
                // Restrigindo as animações para valores permitidos
                if(sprite->currentFrame == range.end){
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
            
        }
        
    }
}

// Função para selecionar qual sprite será manipulada e desenhada
void DrawMario(Mario_t *Mario){
    // Ponteiros para manipular as infos de animação de Mario
    MarioAnimDB_t *currentAnimDB = NULL; 
    AnimData_t *currentAnimData = NULL; 
    MarioSprite_t *activeSprite = &Mario->animations.activeSprite; // Sprite ativa
    
    // Selecionando o banco de dados das animações, conforme o estado atual do Mario e alterando sheet/tamanho dos frames no activeSprite
    switch(Mario->powerUpState){
        case STATE_SMALL:
            currentAnimDB = &Mario->animations.smallMarioAnimDB;
            activeSprite->spriteSheet = Mario->animations.smallMarioSheet;
            activeSprite->frameWidthCut = NORMAL_MARIO_FRAME_WIDHT_CUT;
            activeSprite->frameHeightCut = NORMAL_MARIO_FRAME_HEIGHT_CUT;
            break;
        case STATE_SUPER:
            currentAnimDB = &Mario->animations.superMarioAnimDB;
            activeSprite->spriteSheet = Mario->animations.superMarioSheet;
            activeSprite->frameWidthCut = SUPER_MARIO_FRAME_WIDHT_CUT;
            activeSprite->frameHeightCut = SUPER_MARIO_FRAME_HEIGHT_CUT;
            break;
        case STATE_FIRE:
            currentAnimDB = &Mario->animations.fireMarioAnimDB;
            activeSprite->spriteSheet = Mario->animations.fireMarioSheet;
            activeSprite->frameWidthCut = FIRE_MARIO_FRAME_WIDHT_CUT;
            activeSprite->frameHeightCut = FIRE_MARIO_FRAME_HEIGHT_CUT;
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
        ChangeSpriteTimer(Mario, activeSprite, range); // Função que verifica o momento de alterar o frame, e o faz quando chega
    }
    else{ // Se é frame fixo
        if(Mario->facingRight){ // Virado para direita
            activeSprite->currentFrame = currentAnimData->freezedFrameRight;
        }
        else{ // Esquerda
            activeSprite->currentFrame = currentAnimData->freezedFrameLeft;
        }
    }

    // Desenha a sprite na tela
    ChangeMarioSpritePosition(
        Mario,
        activeSprite,
        activeSprite->frameWidthCut * MARIO_SPRITE_SCALE,
        activeSprite->frameHeightCut * MARIO_SPRITE_SCALE);
}

