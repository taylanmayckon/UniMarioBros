#include "mario.h"
#include "raylib.h"
#include <stdio.h>

// Inicializa banco de animação do Mario pequeno
static MarioAnimDB_t InitSmallMarioDB(void) {
    MarioAnimDB_t db = {0}; // Zera estrutura

    db.idle    = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  4, .freezedFrameRight =  7 }; // Parado
    db.walking = (AnimData_t){ .isLooping = true,
                               .leftAnimFrames  = (FrameRange_t){ 2, 4 },
                               .rightAnimFrames = (FrameRange_t){ 7, 9 } }; // Andando
    db.jumping = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  0, .freezedFrameRight = 11 }; // Pulando
    db.slide   = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  1, .freezedFrameRight = 10 }; // Deslizando
    db.death   = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  5, .freezedFrameRight =  5 }; // Morto

    return db; // Retorna banco
}

// Inicializa banco de animação do Super Mario
static MarioAnimDB_t InitSuperMarioDB(void) {
    MarioAnimDB_t db = {0}; // Zera estrutura

    db.idle    = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  5, .freezedFrameRight =  6 };
    db.walking = (AnimData_t){ .isLooping = true,
                               .leftAnimFrames  = (FrameRange_t){ 3, 5 },
                               .rightAnimFrames = (FrameRange_t){ 6, 8 } };
    db.jumping = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  1, .freezedFrameRight = 10 };
    db.slide   = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  2, .freezedFrameRight =  9 };
    db.crouch  = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  0, .freezedFrameRight = 11 };

    return db; // Retorna banco
}

// Inicializa banco de animação do Fire Mario
static MarioAnimDB_t InitFireMarioDB(void) {
    MarioAnimDB_t db = {0}; // Zera estrutura

    db.idle    = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  6, .freezedFrameRight =  7 };
    db.walking = (AnimData_t){ .isLooping = true,
                               .leftAnimFrames  = (FrameRange_t){ 4, 6 },
                               .rightAnimFrames = (FrameRange_t){ 7, 9 } };
    db.jumping = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  2, .freezedFrameRight = 11 };
    db.slide   = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  3, .freezedFrameRight = 10 };
    db.crouch  = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  1, .freezedFrameRight = 12 };
    db.throw   = (AnimData_t){ .isLooping = false, .freezedFrameLeft =  0, .freezedFrameRight = 13 };

    return db; // Retorna banco
}

// Inicializa uma sprite do Mario
static void InitSprite(MarioSprite_t *sprite,
                       Texture2D texture,
                       Rectangle original_frame_pos_scale,
                       float frameSpeed,
                       float frameTimer,
                       int currentFrame)
{
    sprite->spriteSheet = texture; // Sprite sheet
    sprite->sourceRec   = original_frame_pos_scale; // Retângulo de origem
    sprite->frameSpeed  = frameSpeed; // Velocidade da animação
    sprite->frameTimer  = frameTimer; // Cronômetro
    sprite->currentFrame = currentFrame; // Frame inicial
    sprite->frameWidthCut  = original_frame_pos_scale.width; // Largura do frame
    sprite->frameHeightCut = original_frame_pos_scale.height; // Altura do frame
    sprite->revertAnim = false; // Não reverte animação
}

// Inicializa o Mario
static void InitMario(Mario_t *Mario) {
    printf("[InitMario] Running\n"); // Log

    Mario->position      = (Vector2){300.0f, 300.0f}; // Posição inicial
    Mario->speed         = (Vector2){0.0f, 0.0f};     // Velocidade inicial
    Mario->jumpForce     = -850.0f;                   // Força do pulo
    Mario->lives         = 3;                         // Vidas
    Mario->score         = 0;                         // Pontuação
    Mario->coins         = 0;                         // Moedas
    Mario->invincible    = false;                     // Não invencível
    Mario->canJump       = true;                      // Pode pular
    Mario->canMove       = true;                      // Pode mover
    Mario->facingRight   = true;                      // Virado para direita
    Mario->powerUpState  = STATE_SMALL;               // Começa pequeno
    Mario->actualState   = ACTION_IDLE;               // Parado

    Mario->animations.smallMarioSheet = LoadTexture("assets/textures/mario/smallmario.png"); // Carrega textura pequeno
    Mario->animations.superMarioSheet = LoadTexture("assets/textures/mario/supermario.png"); // Carrega textura super
    Mario->animations.fireMarioSheet  = LoadTexture("assets/textures/mario/firemario.png");  // Carrega textura fogo

    Mario->animations.smallMarioAnimDB = InitSmallMarioDB(); // Inicializa banco pequeno
    Mario->animations.superMarioAnimDB = InitSuperMarioDB(); // Inicializa banco super
    Mario->animations.fireMarioAnimDB  = InitFireMarioDB();  // Inicializa banco fogo

    InitSprite(&Mario->animations.smallMarioSprite,
               Mario->animations.smallMarioSheet,
               (Rectangle){ 0.0f,  0.0f,
                            NORMAL_MARIO_FRAME_WIDHT_CUT,
                            NORMAL_MARIO_FRAME_HEIGHT_CUT },
               0.1f,  // Velocidade do frame
               0.0f,  // Cronômetro
               7      // Frame inicial
    );
    InitSprite(&Mario->animations.superMarioSprite,
               Mario->animations.superMarioSheet,
               (Rectangle){ 0.0f,  0.0f,
                            SUPER_MARIO_FRAME_WIDHT_CUT,
                            SUPER_MARIO_FRAME_HEIGHT_CUT },
               0.1f,
               0.0f,
               6
    );
    InitSprite(&Mario->animations.fireMarioSprite,
               Mario->animations.fireMarioSheet,
               (Rectangle){ 0.0f,  0.0f,
                            FIRE_MARIO_FRAME_WIDHT_CUT,
                            FIRE_MARIO_FRAME_HEIGHT_CUT },
               0.1f,
               0.0f,
               6
    );

    Mario->animations.activeSprite = &Mario->animations.smallMarioSprite; // Começa pequeno

    printf("[InitMario] Finished\n"); // Log
}

// Garante que currentFrame está no intervalo
static void ConstrainIndex(Mario_t *Mario, FrameRange_t range) {
    MarioSprite_t *sprite = Mario->animations.activeSprite; // Sprite ativa

    if (sprite->currentFrame < range.start) {
        sprite->currentFrame = range.start; // Corrige para início
    } else if (sprite->currentFrame > range.end) {
        sprite->currentFrame = range.end;   // Corrige para fim
    }
}

// Atualiza frame da animação em loop
static void ChangeSpriteTimer(Mario_t *Mario, FrameRange_t range) {
    MarioSprite_t *sprite = Mario->animations.activeSprite; // Sprite ativa
    ConstrainIndex(Mario, range); // Garante intervalo

    sprite->frameTimer += GetFrameTime(); // Incrementa timer
    if (sprite->frameTimer >= sprite->frameSpeed) {
        sprite->frameTimer = 0; // Zera timer

        if (Mario->facingRight) {
            sprite->currentFrame++; // Próximo frame direita
            if (sprite->currentFrame > range.end) sprite->currentFrame = range.start; // Loop
        } else {
            sprite->currentFrame--; // Próximo frame esquerda
            if (sprite->currentFrame < range.start) sprite->currentFrame = range.end; // Loop
        }
    }
}

// Seleciona animação e desenha Mario
static void DrawMario(Mario_t *Mario) {
    MarioAnimDB_t *currentAnimDB = NULL; // Banco atual
    AnimData_t    *currentAnimData = NULL; // Dados atuais

    switch (Mario->powerUpState) { // Seleciona banco
        case STATE_SMALL:
            currentAnimDB = &Mario->animations.smallMarioAnimDB;
            Mario->animations.activeSprite = &Mario->animations.smallMarioSprite;
            break;
        case STATE_SUPER:
            currentAnimDB = &Mario->animations.superMarioAnimDB;
            Mario->animations.activeSprite = &Mario->animations.superMarioSprite;
            break;
        case STATE_FIRE:
            currentAnimDB = &Mario->animations.fireMarioAnimDB;
            Mario->animations.activeSprite = &Mario->animations.fireMarioSprite;
            break;
    }

    switch (Mario->actualState) { // Seleciona animação
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
        case ACTION_THROW:
            currentAnimData = &currentAnimDB->throw;
            break;
    }

    if (currentAnimData->isLooping) { // Se animação em loop
        FrameRange_t range = (Mario->facingRight)
                             ? currentAnimData->rightAnimFrames
                             : currentAnimData->leftAnimFrames;
        ChangeSpriteTimer(Mario, range); // Atualiza frame
    } else {
        if (Mario->facingRight)
            Mario->animations.activeSprite->currentFrame = currentAnimData->freezedFrameRight; // Frame fixo direita
        else
            Mario->animations.activeSprite->currentFrame = currentAnimData->freezedFrameLeft;  // Frame fixo esquerda
    }

    float width_scale  = Mario->animations.activeSprite->frameWidthCut  * MARIO_SPRITE_SCALE;  // Largura final
    float height_scale = Mario->animations.activeSprite->frameHeightCut * MARIO_SPRITE_SCALE;  // Altura final

    ChangeMarioSpritePosition(Mario, width_scale, height_scale); // Desenha sprite
}

// Ajusta posição da sprite do Mario
static void ChangeMarioSpritePosition(Mario_t *Mario,
                                      float width_scale,
                                      float height_scale)
{
    MarioSprite_t *sprite = Mario->animations.activeSprite; // Sprite ativa

    float offset_x = width_scale / 2.0f; // Offset X
    float offset_y = height_scale;       // Offset Y

    sprite->destRec.x      = Mario->position.x - offset_x; // Calcula X destino
    sprite->destRec.y      = Mario->position.y - offset_y; // Calcula Y destino
    sprite->destRec.width  = width_scale;                  // Largura destino
    sprite->destRec.height = height_scale;                 // Altura destino

    sprite->sourceRec.x = (float)(sprite->currentFrame * sprite->frameWidthCut); // Atualiza frame

    Vector2 origin = { 0, 0 }; // Origem

    DrawTexturePro(
        sprite->spriteSheet, // Sprite sheet
        sprite->sourceRec,   // Origem
        sprite->destRec,     // Destino
        origin,              // Origem
        0.0f,                // Rotação
        WHITE                // Cor
    );
}

// Retorna retângulo de colisão do Mario
static Rectangle GetMarioCollisionRect(Mario_t *Mario) {
    MarioSprite_t *sprite = Mario->animations.activeSprite; // Sprite ativa
    float width  = sprite->frameWidthCut * MARIO_SPRITE_SCALE; // Largura
    float height = sprite->frameHeightCut * MARIO_SPRITE_SCALE; // Altura

    float x = Mario->position.x - (width / 2.0f); // X do retângulo
    float y = Mario->position.y - height;         // Y do retângulo

    return (Rectangle){
        x + 12.0f,     // Ajuste X
        y - 2.0f,     // Ajuste Y
        width - 24.0f,// Ajuste largura
        height + 2.0f // Ajuste altura
    };
}
