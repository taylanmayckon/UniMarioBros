/*******************************************************************************************
 * UniMarioBros Unificado em um único arquivo main.c (sem textura do chão, mantendo física)
 *
 * Ajustes realizados:
 *  - Não desenha a textura do chão (plataforma 0), apenas mantém sua colisão
 *  - Continua desenhando apenas as plataformas suspensas e as dinâmicas
 *  - Mantém as moedas animadas e a física completa
 *  - Corrige tremulação do Mario arredondando a posição x da câmera
 *
 * Estrutura de pastas esperada:
 *   assets/
 *     textures/
 *       background.png
 *       ItemsAndBlock.png
 *       coin.png
 *       mario/
 *         smallmario.png
 *         supermario.png
 *         firemario.png
 *     audio/
 *       bump.wav
 *       jump.wav
 *
 * Compile com:
 *   gcc -o uni_mariobros main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
 *
 * Ajuste as flags conforme sua instalação do raylib.
 *******************************************************************************************/

// Inclui biblioteca padrão de entrada/saída
#include <stdio.h>
// Inclui biblioteca padrão de alocação de memória
#include <stdlib.h>
// Inclui biblioteca matemática
#include <math.h>
// Inclui cabeçalho da raylib
#include "raylib.h"

// Textura global dos blocos
static Texture2D blocksTexture;

// Estrutura de uma plataforma fixa
typedef struct {
    Rectangle rect;  // Retângulo da plataforma
} Platform;

// Estrutura do cenário do jogo
typedef struct {
    Texture2D background;         // Textura do fundo
    Platform *platforms;          // Vetor de plataformas
    int       platformCount;      // Quantidade de plataformas
    float     scale;              // Fator de escala do fundo
    float     scaledBackgroundWidth; // Largura do fundo escalado
} GameScene;

// Função para criar e inicializar o cenário
static GameScene CreateGameScene(const char *backgroundImagePath) {

    GameScene scene = {0}; // Inicializa a estrutura zerada

    scene.background = LoadTexture(backgroundImagePath); // Carrega textura do fundo
    scene.scale = (float)GetScreenHeight() / (scene.background.height/2); // Calcula escala
    scene.scaledBackgroundWidth = scene.background.width * scene.scale; // Calcula largura escalada

  
    return scene; // Retorna cenário criado
}

// Função para desenhar o cenário
static void DrawGameScene(GameScene scene) {
    Rectangle sourceRec = { 0, 0, scene.background.width, scene.background.height/2 }; // Retângulo de origem do fundo
    Rectangle destRec = { 0, 0, scene.scaledBackgroundWidth, (float)GetScreenHeight() }; // Retângulo de destino

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
static void UnloadGameScene(GameScene scene) {
    UnloadTexture(scene.background); // Libera textura do fundo
    free(scene.platforms);           // Libera vetor de plataformas
}

// Define intervalo de frames para animação
typedef struct {
    int start; // Frame inicial
    int end;   // Frame final
} FrameRange_t;

// Estrutura de dados de animação
typedef struct {
    bool isLooping;              // Se anima em loop
    FrameRange_t leftAnimFrames; // Intervalo para esquerda
    FrameRange_t rightAnimFrames;// Intervalo para direita
    int freezedFrameLeft;        // Frame fixo esquerda
    int freezedFrameRight;       // Frame fixo direita
} AnimData_t;

// Banco de animações para cada estado do Mario
typedef struct {
    AnimData_t idle;     // Parado
    AnimData_t walking;  // Andando
    AnimData_t jumping;  // Pulando
    AnimData_t slide;    // Deslizando
    AnimData_t crouch;   // Agachado
    AnimData_t death;    // Morto
    AnimData_t throw;    // Jogar bola de fogo
} MarioAnimDB_t;

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

// Estados de power-up do Mario
typedef enum {
    STATE_SMALL,  // Pequeno
    STATE_SUPER,  // Grande
    STATE_FIRE    // Fogo
} MarioPowerUpStates_t;

// Ações do Mario
typedef enum {
    ACTION_IDLE,    // Parado
    ACTION_WALKING, // Andando
    ACTION_JUMPING, // Pulando
    ACTION_SLIDE,   // Deslizando
    ACTION_CROUCH,  // Agachado
    ACTION_DEATH,   // Morto
    ACTION_THROW    // Jogar bola de fogo
} MarioActionStates_t;

// Estrutura da sprite do Mario
typedef struct {
    Texture2D spriteSheet;    // Sprite sheet
    Rectangle sourceRec;      // Retângulo de origem
    Rectangle destRec;        // Retângulo de destino
    int       currentFrame;   // Frame atual
    float     frameSpeed;     // Velocidade da animação
    float     frameTimer;     // Cronômetro
    bool      revertAnim;     // Reverter animação
    float     frameWidthCut;  // Largura do frame
    float     frameHeightCut; // Altura do frame
} MarioSprite_t;

// Estrutura de animações do Mario
typedef struct {
    Texture2D       smallMarioSheet;   // Sprite sheet pequeno
    Texture2D       superMarioSheet;   // Sprite sheet super
    Texture2D       fireMarioSheet;    // Sprite sheet fogo

    MarioSprite_t   smallMarioSprite;  // Sprite pequeno
    MarioSprite_t   superMarioSprite;  // Sprite super
    MarioSprite_t   fireMarioSprite;   // Sprite fogo

    MarioSprite_t  *activeSprite;      // Sprite ativa

    MarioAnimDB_t   smallMarioAnimDB;  // Banco animação pequeno
    MarioAnimDB_t   superMarioAnimDB;  // Banco animação super
    MarioAnimDB_t   fireMarioAnimDB;   // Banco animação fogo
} MarioAnimation_t;

// Estrutura principal do Mario

typedef struct {
    Vector2                position;       // Posição
    Vector2                speed;          // Velocidade
    float                  jumpForce;      // Força do pulo
    int                    lives;          // Vidas
    int                    score;          // Pontuação
    int                    coins;          // Moedas
    bool                   invincible;     // Invencível
    bool                   canJump;        // Pode pular
    bool                   canMove;        // Pode mover
    bool                   facingRight;    // Virado para direita
    MarioPowerUpStates_t   powerUpState;   // Estado power-up
    MarioActionStates_t    actualState;    // Estado atual
    MarioAnimation_t       animations;     // Animações
} Mario_t;

// Define largura do frame do Mario normal
#define NORMAL_MARIO_FRAME_WIDHT_CUT  16.0f
// Define altura do frame do Mario normal
#define NORMAL_MARIO_FRAME_HEIGHT_CUT 16.f
// Define largura do frame do Super Mario
#define SUPER_MARIO_FRAME_WIDHT_CUT  16.0f
// Define altura do frame do Super Mario
#define SUPER_MARIO_FRAME_HEIGHT_CUT 30.0f
// Define largura do frame do Fire Mario
#define FIRE_MARIO_FRAME_WIDHT_CUT  16.0f
// Define altura do frame do Fire Mario
#define FIRE_MARIO_FRAME_HEIGHT_CUT 31.0f
// Escala global do Mario
#define MARIO_SPRITE_SCALE 3.5f

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

// Número máximo de plataformas dinâmicas
#define MAX_PHYS_PLATFORMS 10
// Número máximo de moedas
#define MAX_COINS 10

// Gravidade
static const float GRAVITY     = 1800.0f;
// Velocidade horizontal do Mario
static const float MOVE_SPEED  = 300.0f;
// Força do pulo
static const float JUMP_FORCE  = -870.0f;

// Estrutura de plataforma dinâmica
typedef struct {
    Rectangle rect;        // Retângulo da plataforma
    float     originalY;   // Y original
    float     bounceOffset;// Offset do bounce
    bool      bouncing;    // Está quicando
    int       bounceDir;   // Direção do bounce
    float     bounceSpeed; // Velocidade do bounce
} PhysPlatform_t;

// Estrutura da moeda animada
typedef struct {
    Vector2   position;    // Posição
    Rectangle sourceRec;   // Retângulo de origem
    int       currentFrame;// Frame atual
    int       frameCounter;// Contador de frames
    int       totalFrames; // Total de frames
    int       frameSpeed;  // Velocidade da animação
    float     frameWidth;  // Largura do frame
    float     frameHeight; // Altura do frame
    bool      active;      // Ativa ou não
} Coin_t;

// Som de bounce
static Sound bumpSound;
// Som de pulo
static Sound jumpSound;
// Textura das moedas
static Texture2D coinAtlas;

// Contador de plataformas dinâmicas
static int physPlatCount = 0;
// Vetor de plataformas dinâmicas
static PhysPlatform_t physPlatforms[MAX_PHYS_PLATFORMS];

// Contador de moedas
static int coinCount = 0;
// Vetor de moedas
static Coin_t coins[MAX_COINS];

// Prototipação de funções auxiliares
static Rectangle GetMarioCollisionRect(Mario_t *Mario);
static void HandleMarioPlatformCollisions(Mario_t *Mario);
static Coin_t CreateCoin(Vector2 position);
static void UpdateCoins(Mario_t *Mario);
static void DrawCoins(void);

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

// Trata colisão Mario/plataforma
static void HandleMarioPlatformCollisions(Mario_t *Mario) {
    Rectangle mRect = GetMarioCollisionRect(Mario); // Retângulo do Mario

    for (int i = 0; i < physPlatCount; i++) {
        PhysPlatform_t *p = &physPlatforms[i]; // Plataforma
        Rectangle pRect = p->rect;             // Retângulo da plataforma

        if (CheckCollisionRecs(mRect, pRect)) { // Se colidiu
            if (Mario->speed.y > 0 && (mRect.y + mRect.height - (Mario->speed.y * GetFrameTime())) <= pRect.y) {
                Mario->position.y = pRect.y; // Coloca Mario em cima
                Mario->speed.y = 0;          // Zera velocidade Y
                Mario->canJump = true;       // Pode pular
            }
            else if (Mario->speed.y < 0 && (mRect.y) <= (pRect.y + pRect.height) &&
                     (mRect.y + mRect.height) > (pRect.y + pRect.height))
            {
                Mario->speed.y = 0; // Zera velocidade Y

                if (i > 0 && !p->bouncing) { // Se não for chão
                    p->bouncing = true;      // Ativa bounce
                    p->bounceDir = 1;        // Sobe
                    p->bounceOffset = 0.0f;  // Zera offset
                    PlaySound(bumpSound);    // Toca som
                }
            }
        }
    }
}

// Cria moeda
static Coin_t CreateCoin(Vector2 position) {
    return (Coin_t){
        .position     = position, // Posição
        .frameCounter = 0,        // Contador
        .currentFrame = 0,        // Frame inicial
        .totalFrames  = 4,        // Total de frames
        .frameSpeed   = 8,        // Velocidade
        .frameWidth   = 16.0f,    // Largura
        .frameHeight  = 16.0f,    // Altura
        .sourceRec    = { 0.0f, 0.0f, 16.0f, 16.0f }, // Origem
        .active       = true      // Ativa
    };
}

// Atualiza moedas
static void UpdateCoins(Mario_t *Mario) {
    Rectangle mRect = GetMarioCollisionRect(Mario); // Retângulo do Mario

    for (int i = 0; i < coinCount; i++) {
        Coin_t *c = &coins[i]; // Moeda
        if (!c->active) continue; // Se não ativa, pula

        Rectangle coinRect = {
            c->position.x + 3.0f,
            c->position.y + 3.0f,
            c->frameWidth * 3.0f - 6.0f,
            c->frameHeight * 3.0f - 6.0f
        };

        if (CheckCollisionRecs(mRect, coinRect)) { // Se colidiu
            c->active = false; // Desativa moeda
            Mario->coins++;    // Incrementa moedas
            Mario->score += 100; // Pontuação
        }

        c->frameCounter++; // Incrementa contador
        if (c->frameCounter >= (int)(60 / c->frameSpeed)) {
            c->frameCounter = 0; // Zera contador
            c->currentFrame = (c->currentFrame + 1) % c->totalFrames; // Próximo frame
            c->sourceRec.x = c->currentFrame * c->frameWidth; // Atualiza origem
        }
    }
}

// Desenha moedas
static void DrawCoins(void) {
    for (int i = 0; i < coinCount; i++) {
        Coin_t *c = &coins[i]; // Moeda
        if (!c->active) continue; // Se não ativa, pula

        DrawTexturePro(
            coinAtlas, // Textura moeda
            c->sourceRec, // Origem
            (Rectangle){ c->position.x, c->position.y,
                         c->frameWidth  * 3.0f,
                         c->frameHeight * 3.0f },
            (Vector2){ 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }
}

// Função principal
int main(void) {
    const int screenWidth  = 990; // Largura da janela
    const int screenHeight = 720; // Altura da janela
    InitWindow(screenWidth, screenHeight, "UniMarioBros Unificado (arquivo único)"); // Inicializa janela
    InitAudioDevice(); // Inicializa áudio
    SetTargetFPS(60);  // Define FPS

    blocksTexture = LoadTexture("assets/textures/ItemsAndBlock.png"); // Carrega textura blocos

    GameScene scene = CreateGameScene("assets/textures/background.png"); // Cria cenário

    bumpSound = LoadSound("assets/audio/bump.wav"); // Carrega som bump
    jumpSound = LoadSound("assets/audio/jump.wav"); // Carrega som pulo

    coinAtlas = LoadTexture("assets/textures/coin.png"); // Carrega textura moeda

    physPlatCount = 0; // Zera contador plataformas

    physPlatforms[physPlatCount++] = (PhysPlatform_t){
        .rect        = { 0.0f, 545.0f, 1200.0f, 120.0f },
        .originalY   = 545.0f,
        .bounceOffset=  0.0f,
        .bouncing    = false,
        .bounceDir   =  0,
        .bounceSpeed =  0.0f
    };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){
        .rect        = { 500.0f, 355.0f, 60.0f, 57.0f },
        .originalY   = 355.0f,
        .bounceOffset=  0.0f,
        .bouncing    = false,
        .bounceDir   =  0,
        .bounceSpeed =  0.0f
    };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){
        .rect        = { 556.0f, 355.0f, 60.0f, 57.0f },
        .originalY   = 355.0f,
        .bounceOffset=  0.0f,
        .bouncing    = false,
        .bounceDir   =  0,
        .bounceSpeed =  0.0f
    };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){
        .rect        = { 662.0f, 355.0f, 60.0f, 57.0f },
        .originalY   = 355.0f,
        .bounceOffset=  0.0f,
        .bouncing    = false,
        .bounceDir   =  0,
        .bounceSpeed =  0.0f
    };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){
        .rect        = { 712.0f, 355.0f, 60.0f, 57.0f },
        .originalY   = 355.0f,
        .bounceOffset=  0.0f,
        .bouncing    = false,
        .bounceDir   =  0,
        .bounceSpeed =  0.0f
    };
  

    coinCount = 0; // Zera moedas
    coins[coinCount++] = CreateCoin((Vector2){500.0f, 280.0f}); // Cria moeda
    coins[coinCount++] = CreateCoin((Vector2){556.0f, 280.0f});
    coins[coinCount++] = CreateCoin((Vector2){662.0f, 280.0f});
    coins[coinCount++] = CreateCoin((Vector2){712.0f, 280.0f});

    Mario_t Mario; // Declara Mario
    InitMario(&Mario); // Inicializa Mario

    Camera2D camera = { 0 }; // Inicializa câmera
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }; // Offset da câmera
    camera.zoom   = 1.0f; // Zoom
    camera.rotation = 0.0f; // Rotação

    while (!WindowShouldClose()) { // Loop principal
        float delta = GetFrameTime(); // Tempo do frame

        bool isMoving = false; // Flag de movimento

        if (IsKeyDown(KEY_RIGHT) && Mario.canMove) { // Move direita
            Rectangle mNext = GetMarioCollisionRect(&Mario);
            mNext.x += MOVE_SPEED * delta;

            bool collision = false;
            for (int i = 0; i < physPlatCount; i++) {
                if (CheckCollisionRecs(mNext, physPlatforms[i].rect)) {
                    collision = true;
                    break;
                }
            }
            if (!collision) {
                Mario.position.x += MOVE_SPEED * delta;
                Mario.facingRight = true;
                isMoving = true;
            }
        }
        if (IsKeyDown(KEY_LEFT) && Mario.canMove) { // Move esquerda
            Rectangle mNext = GetMarioCollisionRect(&Mario);
            mNext.x -= MOVE_SPEED * delta;

            bool collision = false;
            for (int i = 0; i < physPlatCount; i++) {
                if (CheckCollisionRecs(mNext, physPlatforms[i].rect)) {
                    collision = true;
                    break;
                }
            }
            if (!collision) {
                Mario.position.x -= MOVE_SPEED * delta;
                Mario.facingRight = false;
                isMoving = true;
            }
        }
        if (IsKeyPressed(KEY_SPACE) && Mario.canJump) { // Pulo
            Mario.speed.y = JUMP_FORCE;
            Mario.canJump = false;
            Mario.actualState = ACTION_JUMPING;
            PlaySound(jumpSound);
        }

        Mario.speed.y += GRAVITY * delta; // Aplica gravidade
        Mario.position.y += Mario.speed.y * delta; // Atualiza Y

        HandleMarioPlatformCollisions(&Mario); // Colisão vertical

        for (int i = 1; i < physPlatCount; i++) { // Atualiza bounce
            PhysPlatform_t *p = &physPlatforms[i];
            if (p->bouncing) {
                if (p->bounceDir == 1) {
                    p->bounceOffset -= 200.0f * delta;
                    if (p->bounceOffset <= -15.0f) {
                        p->bounceOffset = -15.0f;
                        p->bounceDir = -1;
                    }
                } else {
                    p->bounceOffset += 200.0f * delta;
                    if (p->bounceOffset >= 0.0f) {
                        p->bounceOffset = 0.0f;
                        p->bouncing    = false;
                    }
                }
                p->rect.y = p->originalY + p->bounceOffset;
            }
        }

        UpdateCoins(&Mario); // Atualiza moedas

        if (!Mario.canJump) {
            Mario.actualState = ACTION_JUMPING;
        } else {
            if (isMoving) {
                Mario.actualState = ACTION_WALKING;
            } else {
                Mario.actualState = ACTION_IDLE;
            }
        }

        float camTargetX = roundf(Mario.position.x); // Arredonda X da câmera
        camera.target = (Vector2){ camTargetX, 290.0f }; // Atualiza alvo da câmera
        if (camera.target.x < screenWidth / 2.0f) camera.target.x = screenWidth / 2.0f; // Limita esquerda

        BeginDrawing(); // Começa desenho
        ClearBackground((Color){92, 148, 252, 255}); // Limpa fundo com cor

        BeginMode2D(camera); // Modo 2D
            DrawGameScene(scene); // Desenha cenário
            DrawCoins();         // Desenha moedas

            Rectangle blockSource = { 271.0f, 191.0f, 16.0f, 17.0f }; // Sprite bloco
            for (int i = 1; i < physPlatCount; i++) { // Desenha plataformas dinâmicas
                DrawTexturePro(
                    blocksTexture,
                    blockSource,
                    physPlatforms[i].rect,
                    (Vector2){ 0.0f, 0.0f },
                    0.0f,
                    WHITE
                );
            }

            DrawMario(&Mario); // Desenha Mario

        EndMode2D(); // Fim modo 2D

        EndDrawing(); // Fim desenho
    }

    UnloadGameScene(scene); // Libera cenário

    UnloadSound(bumpSound); // Libera som bump
    UnloadSound(jumpSound); // Libera som pulo

    UnloadTexture(blocksTexture); // Libera textura blocos
    UnloadTexture(coinAtlas);     // Libera textura moedas

    UnloadTexture(Mario.animations.smallMarioSheet); // Libera textura Mario pequeno
    UnloadTexture(Mario.animations.superMarioSheet); // Libera textura Mario super
    UnloadTexture(Mario.animations.fireMarioSheet);  // Libera textura Mario fogo

    CloseAudioDevice(); // Fecha áudio
    CloseWindow();      // Fecha janela

    return 0; // Fim do programa
}
