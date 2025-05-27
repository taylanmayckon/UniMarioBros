#include "raylib.h"
#include "mario.h"
#include <stdio.h>

// Definições para a sprite walk right (174x16 total -> 29x16/frame)
#define WALK_RIGHT_FRAME_WIDHT_CUT 29.0f
#define WALK_RIGHT_FRAME_HEIGHT_CUT 16.0f

void InitSprite(MarioSprite_t *sprite, char *asset_dir, Rectangle original_frame_pos_scale, float frameSpeed, float frameTimer, int frameCount, int currentFrame){
    sprite->spriteSheet = LoadTexture(asset_dir);
    if (sprite->spriteSheet.id == 0) {
        printf("[InitSprite] ERRO: Não foi possível carregar a textura: %s\n", asset_dir);
        return;
    }
    // Depurando
    sprite->sourceRec = original_frame_pos_scale;
    sprite->frameSpeed = frameSpeed;
    sprite->frameTimer = frameTimer;
    sprite->frameCount = frameCount;
    sprite->currentFrame = currentFrame;
}

void InitMario(Mario_t *Mario){
    // Função genérica para inicializar a struct Mario
    printf("[InitMario] Running\n");

    Mario->position = (Vector2){300.0f, 300.0f}; // Posição inicial
    Mario->facingRight = true; // Virado para direita
    Mario->actualState = ACTION_IDLE; // Parado

    // Iniciando cada sprite da struct de animações
    // Andando para direita
    InitSprite(&Mario->animations.walkRight, // Ponteiro para a sprite 
        "assets/textures/mario/mario1-walkright.png", // Diretorio da spriteSheet
        (Rectangle){0.0f, 0.0f,// Posição da origem dos frames
                    WALK_RIGHT_FRAME_WIDHT_CUT, WALK_RIGHT_FRAME_HEIGHT_CUT}, // (ESCALA) Largura e altura de cada frame (ajustar depois individualmente)
        0.2f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        6, // Quantidade total de frames
        0); // Frame atual
    
    // Andando para esquerda
    InitSprite(&Mario->animations.walkLeft, // Ponteiro para a sprite 
        "assets/textures/mario/mario1-walkleft.png", // Diretorio da spriteSheet
        (Rectangle){0.0f, 0.0f,// Posição da origem dos frames
                    WALK_RIGHT_FRAME_WIDHT_CUT, WALK_RIGHT_FRAME_HEIGHT_CUT}, // (ESCALA) Largura e altura de cada frame (ajustar depois individualmente)
        0.2f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        6, // Quantidade total de frames
        5); // Frame atual (Left é invertido, o parado é o ultimo)
    
    printf("[InitMario] Finished\n");
}

void ChangeMarioSpritePosition(Mario_t *Mario, MarioSprite_t *sprite, float width, float height){
    // Função para mover a sprite de lugar e desenhá-la
    // Posição
    sprite->destRec.x = Mario->position.x;
    sprite->destRec.y = Mario->position.y;
    // Proporção, tem que ser ajustada corretamente depois
    sprite->destRec.width = width;
    sprite->destRec.height = height;

    // Alterna o frame da sprite com base no contador
    sprite->sourceRec.x = (float)sprite->currentFrame * WALK_RIGHT_FRAME_WIDHT_CUT;
    printf("[ChangeMarioSpritePosition] SourceRec.x: %.2f\n", sprite->sourceRec.x);
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

void ChangeSpriteTimer(Mario_t *Mario, MarioSprite_t *sprite){
    // Função para CONTROLAR a temporização dos frames das sprites
    // Acumula o tempo que passou desde o ultimo frame
    // Na pratica é um cronometro
    sprite->frameTimer += GetFrameTime();
    // Depuração
    printf("[ChangeSpriteTimer] Timer: %.2f, Frame Atual:%d\n", sprite->frameTimer, sprite->currentFrame);
    printf("[ChangeSpriteTimer] GetFrameTime(): %f\n", GetFrameTime());

    // Verificando se tá na hora de alterar o frame da animação
    if(sprite->frameTimer >= sprite->frameSpeed){
        sprite->frameTimer = 0; // Reinicia o cronometro

        if(Mario->facingRight){ // Incrementa quando é para direita
            sprite->currentFrame++; // Avança para o próximo frmae
        }
        else{
            sprite->currentFrame--; // Decrementa quando é para esquerda
        }
        
        
        // Restringindo as animações para valores permitidos
        if(sprite->currentFrame >= sprite->frameCount && Mario->facingRight){ // Pra direita
            sprite->currentFrame = 1; // O 0 é ele parado, andando é 1 em diante
        }
        else if(sprite->currentFrame <= 0 && !Mario->facingRight){ // Pra esquerda
            sprite->currentFrame = sprite->frameCount-2; // O frameCount-1 é parado, andando é -2 até 0
        }
    }
}

void IdleMario(Mario_t *Mario){
    printf("[IdleMario] Running\n");
    // Virado para direita:
    if(Mario->facingRight){
        Mario->animations.walkRight.currentFrame=0; // Frame dele parado
        ChangeMarioSpritePosition(Mario, 
            &Mario->animations.walkRight, // Ponteiro para a sprite a ser modificada
            WALK_RIGHT_FRAME_WIDHT_CUT*4, WALK_RIGHT_FRAME_HEIGHT_CUT*4); // Proporção do desenho
    }
    // Virado para esquerda
    else{
        Mario->animations.walkLeft.currentFrame=5; // Frame dele parado
        ChangeMarioSpritePosition(Mario, 
            &Mario->animations.walkLeft, // Ponteiro para a sprite a ser modificada
            WALK_RIGHT_FRAME_WIDHT_CUT*4, WALK_RIGHT_FRAME_HEIGHT_CUT*4); 
    }
}

void WalkingMario(Mario_t *Mario){
    // Virado para direita:
    if(Mario->facingRight){
        ChangeSpriteTimer(Mario, &Mario->animations.walkRight);
        ChangeMarioSpritePosition(Mario, 
            &Mario->animations.walkRight, // Ponteiro para a sprite a ser modificada
            WALK_RIGHT_FRAME_WIDHT_CUT*4, WALK_RIGHT_FRAME_HEIGHT_CUT*4); // Proporção do desenho
    }
    // Virado para esquerda:
    else{
        ChangeSpriteTimer(Mario, &Mario->animations.walkLeft);
        ChangeMarioSpritePosition(Mario, &Mario->animations.walkLeft, // Ponteiro para a sprite a ser modificada
        WALK_RIGHT_FRAME_WIDHT_CUT*4, WALK_RIGHT_FRAME_HEIGHT_CUT*4); // Proporção do desenho
    }
}

void DrawMario(Mario_t *Mario){
    // Função para selecionar qual sprite será manipulada e desenhada
    switch(Mario->actualState){
        // Caso esteja parado
        case ACTION_IDLE:
            IdleMario(Mario);
            break;
        case ACTION_WALKING:
            WalkingMario(Mario);
            break;
        case ACTION_JUMPING:

            break;
        case ACTION_FALLING:

            break;
        default:
            printf("[UpdateMarioSprite] ERRO!");
            break;
    }
}

