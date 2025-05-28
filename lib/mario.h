#ifndef MARIO_H
#define MARIO_H

// Enum para os estados do Mário (Quando ele pega flor, etc...)
typedef enum{
    STATE_SMALL, // Mario pequeno (padrão)
    STATE_BIG, // Mario grande
} MarioPowerUpStates_t;

// Enum para as ações do Mario (pular, andar, cair...)
typedef enum{
    ACTION_IDLE, // Mario parado
    ACTION_WALKING, // Mario andando
    ACTION_JUMPING, // Mario pulando
    ACTION_FALLING // Mario caindo
} MarioActionStates_t;

// Struct para as animações do Mário
typedef struct{
    Texture2D spriteSheet; // Local da textura da animação
    Rectangle sourceRec; // Retangulo de origem (o corte da sprite)
    Rectangle destRec; // Retangulo de destino: pos_x, pos_y, largura, altura
    int frameCount; // Total de frames (max)
    int currentFrame; // Frame atual (0->frameCount)
    float frameSpeed; // Duração de cada frame (ex: 1s)
    float frameTimer; // Contador para animação atual (conta quantos segundos já foram até o frameSpeed)
} MarioSprite_t;

// Struct que armazena as informações das ações do Mário
typedef struct{
    //MarioSprite_t idleAnim; // Parado
    MarioSprite_t walkRight; // Andando para direita
    MarioSprite_t walkLeft; // Andando para direita
    MarioSprite_t jumpAnim; // Pulo
} MarioAnimation_t;

// Struct principal do Mario
typedef struct{
    Vector2 position; // Posição atual do Mario (x, y)
    Vector2 speed; // Velocidade atual do Mario (x, y)
    float jumpForce; // Força do pulo (seria a aceleração)
    int lives; // Contador de vidas
    int score; // Pontuação
    int coins; // Quant. de moedas
    bool canJump; // Booleano para indicar se pode pular ou não
    bool facingRight; // Se esta olhando para a direita ou não (1: direita, 0: não)
    MarioPowerUpStates_t powerUpState; // Estado atual do Mario (ex: normal, grande)
    MarioActionStates_t actualState; // Ação atual do Mario (parado, correndo, pulando...)
    MarioAnimation_t animations; // Todas as animações agrupadas
} Mario_t;

// Protótipos de funções:
void InitSprite(MarioSprite_t *sprite, char *asset_dir, Rectangle original_frame_pos_scale, float frameSpeed, float frameTimer, int frameCount, int currentFrame);
void InitMario(Mario_t *Mario);
void ChangeMarioSpritePosition(Mario_t *Mario, MarioSprite_t *sprite, float width, float height);
void ChangeSpriteTimer(Mario_t *Mario, MarioSprite_t *sprite);
void IdleMario(Mario_t *Mario);
void WalkingMario(Mario_t *Mario);
void DrawMario(Mario_t *Mario);

#endif