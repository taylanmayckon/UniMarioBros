#ifndef PLATFORM_H
#define PLATFORM_H

#include "raylib.h"

// Número máximo de plataformas dinâmicas
#define MAX_PHYS_PLATFORMS 10

// Gravidade
extern const float GRAVITY;
// Velocidade horizontal do Mario
extern const float MOVE_SPEED;
// Força do pulo
extern const float JUMP_FORCE;

// Estrutura de plataforma dinâmica
typedef struct {
    Rectangle rect;        // Retângulo da plataforma
    float     originalY;   // Y original
    float     bounceOffset;// Offset do bounce
    bool      bouncing;    // Está quicando
    int       bounceDir;   // Direção do bounce
    float     bounceSpeed; // Velocidade do bounce
} PhysPlatform_t;

// Contador de plataformas dinâmicas
extern int physPlatCount;
// Vetor de plataformas dinâmicas
extern PhysPlatform_t physPlatforms[MAX_PHYS_PLATFORMS];

// Prototipação de funções auxiliares
static void HandleMarioPlatformCollisions(struct Mario_t *Mario);

#endif // PLATFORM_H
