#ifndef MARIO_H
#define MARIO_H

// Struct para as animações do Mário
typedef struct{
    Texture2D spriteSheet; // Local do arquivo da sprite
    Rectangle sourceRec; // Tamanho de cada frame do sprite 
    Rectangle destRec; // Retangulo de destino: pos_x, pos_y, largura, altura
    int currentFrame; // Frame atual (0->max)
    int frameCount; // Total de frames (max)
    float frameSpeed; // Duração de cada animação (ex: 1s)
    float frameTimer; // Contador para animação atual (conta quantos ticks já foram até o frameSpeed)
} MarioSprite_t;

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

// Struct principal do Mario
typedef struct{
    Vector2 position; // Posição atual do Mario (x, y)
    Vector2 speed; // Velocidade atual do Mario (x, y)
    float jumpForce; // Penso que vá ser para controlar a velocidade do pulo (começa alto e blablabla)
    int lives; // Contador de vidas
    bool canJump; // Booleano para indicar se pode pular ou não
    MarioPowerUpStates_t powerUpState; // Estado atual do Mario (se houver) (ex: normal, grande)
    // Variáveis que armazenam todas as infos sobre as animações (sprite, tamanho, etc)
    MarioSprite_t idleAnim; // Parado
    MarioSprite_t walkRight; // Andando para direita
    MarioSprite_t jumpAnim; // Pulo
} Mario_t;

// Promissoras structs:
// MarioAnimationTimes_t -> Armazena as temporizações de cada animação do Mario vai servir 
//                          para dizer quanto tempo cada frame da sprite é exibido


// Protótipos de funções:
void InitMario(Mario_t *Mario);
void DrawMario(Mario_t *Mario);
void UpdateMario(Mario_t *Mario);

#endif