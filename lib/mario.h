#ifndef MARIO_H
#define MARIO_H

#include "mario_animdb.h"

// Enum para os estados do Mário (Quando ele pega flor, etc...)
typedef enum{
    STATE_SMALL, // Mario pequeno (padrão)
    STATE_SUPER, // Mario grande
    STATE_FIRE, // Mario que taca bola de fogo
} MarioPowerUpStates_t;

// Enum para as ações do Mario (pular, andar, deslizar...)
typedef enum{
    ACTION_IDLE, // Mario parado
    ACTION_WALKING, // Mario andando
    ACTION_JUMPING, // Mario pulando
    ACTION_SLIDE, // Mario deslizando (parando de correr)
    ACTION_CROUCH, // Mario abaixado
    ACTION_DEATH, // Morte
    ACTION_THROW, // Jogar bola de fogo
} MarioActionStates_t;

// Struct para as animações do Mário
typedef struct{
    Texture2D spriteSheet; // Local da textura da animação
    Rectangle sourceRec; // Retangulo de origem (o corte da sprite)
    Rectangle destRec; // Retangulo de destino: pos_x, pos_y, largura, altura
    int currentFrame; // Frame atual (0->frameCount)
    float frameSpeed; // Duração de cada frame (ex: 1s)
    float frameTimer; // Contador para animação atual (conta quantos segundos já foram até o frameSpeed)
    bool revertAnim; // Booleano para inverter animação para tornar mais suave a transição dos frames
    float frameWidthCut; // Largura do corte de cada frame
    float frameHeightCut; // Altura do corte de cada frame
} MarioSprite_t;

// Struct que armazena as informações das ações do Mário
typedef struct{
    // Renderizam os frames do Mario andando, pulando, agachando e deslizando
    Texture2D smallMarioSheet;
    Texture2D superMarioSheet;
    Texture2D fireMarioSheet;
    // Sprites de cada estado do Mario
    MarioSprite_t smallMarioSprite;
    MarioSprite_t superMarioSprite;
    MarioSprite_t fireMarioSprite;
    // Ponteiro para a sprite que será desenhada e animada
    MarioSprite_t *activeSprite; 
    // Banco de dados de cada animação do Mario com base no PowerUp
    MarioAnimDB_t smallMarioAnimDB; // Mario normal
    MarioAnimDB_t superMarioAnimDB; // Super mario
    MarioAnimDB_t fireMarioAnimDB; // Fire mario
} MarioAnimation_t;


// Struct principal do Mario
typedef struct{
    Vector2 position; // Posição atual do Mario (x, y)
    Vector2 speed; // Velocidade atual do Mario (x, y)
    int lives; // Contador de vidas
    int score; // Pontuação
    int coins; // Quant. de moedas
    bool invincible; // Quando ele é atacado fica invencível se tiver cogumelo
    bool canMove; // Booleano para indicar se pode se mover ou não 
    bool isDying; // Booleano para indicar se está morrendo
    bool facingRight; // Se esta olhando para a direita ou não (1: direita, 0: não)
    MarioPowerUpStates_t powerUpState; // Estado atual do Mario (ex: normal, grande)
    MarioActionStates_t actualState; // Ação atual do Mario (parado, correndo, pulando...)
    MarioAnimation_t animations; // Todas as animações agrupadas
} Mario_t;

// Protótipos de funções:
void UpdateMario(Mario_t *Mario);
void InitSprite(MarioSprite_t *sprite, Texture2D texture, Rectangle original_frame_pos_scale, float frameSpeed, float frameTimer, int currentFrame);
void InitMario(Mario_t *Mario);
void ChangeMarioSpritePosition(Mario_t *Mario, float width_scale, float height_scale);
void ChangeSpriteTimer(Mario_t *Mario, FrameRange_t range);
void deathAnim(Mario_t *Mario, int frame_index);
void DrawMario(Mario_t *Mario);

#endif