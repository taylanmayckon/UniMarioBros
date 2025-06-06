#ifndef MARIO_H
#define MARIO_H

#include "raylib.h"

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
static MarioAnimDB_t InitSmallMarioDB(void);
// Inicializa banco de animação do Super Mario
static MarioAnimDB_t InitSuperMarioDB(void);
// Inicializa banco de animação do Fire Mario
static MarioAnimDB_t InitFireMarioDB(void);

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

// Prototipação de funções
static void InitSprite(MarioSprite_t *sprite,
                       Texture2D texture,
                       Rectangle original_frame_pos_scale,
                       float frameSpeed,
                       float frameTimer,
                       int currentFrame);

static void InitMario(Mario_t *Mario);
static void ChangeMarioSpritePosition(Mario_t *Mario,
                                      float width_scale,
                                      float height_scale);
static void ConstrainIndex(Mario_t *Mario, FrameRange_t range);
static void ChangeSpriteTimer(Mario_t *Mario, FrameRange_t range);
static void DrawMario(Mario_t *Mario);

// Prototipação de colisão
static Rectangle GetMarioCollisionRect(Mario_t *Mario);

#endif // MARIO_H
