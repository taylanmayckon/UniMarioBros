#ifndef MARIO_H
#define MARIO_H

// Struct para as animações do Mário
typedef struct{
    Texture2D spriteSheet; // Local do arquivo da sprite
    Rectangle sourceRec; // Tamanho de cada frame do sprite 
    Rectangle destRec; // Retangulo de destino: pos_x, pos_y, largura, altura
    int currentFrame; // Frame atual
    int frameCount; // Total de frames
    int frameSpeed; 
    int frameCounter;
} MarioSprite_t;

// Enum para os estados do Mário (Quando ele pega flor, etc...)
typedef enum{
    STATE_SMALL,
    STATE_BIG,
} MarioPowerUpStates_t;

// Enum para as ações do Mario (pular, andar, cair...)
typedef enum{
    ACTION_IDLE,
    ACTION_WALKING,
    ACTION_JUMPING,
    ACTION_FALLING
} MarioActionStates_t;

// Struct principal do Mario
typedef struct{
    Vector2 position; // Posição atual 
    Vector2 speed; // Velocidade atual 
    float jumpForce;
    int lives;
    bool canJump;
    MarioPowerUpStates_t powerUpState; // Estado atual do Mario (se houver)
    MarioSprite_t idleAnim; // Animação para quando estiver barato
    MarioSprite_t walkRight; // Animação para quando estiver PARA A DIREITA
    MarioSprite_t jumpAnim; // Animação de pulo
} Mario_t;


#endif