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
// Quantidade de plataformas
extern int physPlatCount;

// Estrutura de plataforma dinâmica
typedef struct {
    Rectangle rect;        // Retângulo da plataforma
    float     originalY;   // Y original
    float     bounceOffset;// Offset do bounce
    bool      bouncing;    // Está quicando
    int       bounceDir;   // Direção do bounce
    float     bounceSpeed; // Velocidade do bounce
} PhysPlatform_t;

// Prototipação de funções auxiliares
void InitPlatforms(PhysPlatform_t *physPlatforms);
void UpdatePlatforms(PhysPlatform_t *physPlatform);
void DrawBlocks(PhysPlatform_t *physPlatforms, Texture2D block1);
// void HandleMarioPlatformCollisions(Mario_t *Mario, PhysPlatform_t *physPlatforms, Sound bumpSound);

#endif // PLATFORM_H
