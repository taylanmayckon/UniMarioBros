#include <stdbool.h>
#include "raylib.h"
#include "platform.h"

// Definições das constantes
const float GRAVITY     = 1800.0f;
const float MOVE_SPEED  = 300.0f;
const float JUMP_FORCE  = -870.0f;

// Contador de plataformas dinâmicas
int physPlatCount = 0;

void InitPlatforms(PhysPlatform_t *physPlatforms){
    physPlatCount = 0;
    // As 4 primeiras plataformas são o chão, não dinâmicas
       // Plataformas de chão
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { 0.0f, 445.0f, 1005.0f, 120.0f }, .originalY = 445.0f, .typeBlock = 0 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { 1130.0f, 445.0f, 400.0f, 120.0f }, .originalY = 445.0f, .typeBlock = 0 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { 1695.0f, 445.0f, 720.0f, 120.0f }, .originalY = 445.0f, .typeBlock = 0 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { 2540.0f, 445.0f, 1210.0f, 120.0f }, .originalY = 445.0f, .typeBlock = 0 };

    // Plataformas lado a lado (grupo 1)
    float startX = 580.0f;
    for (int i = 0; i < 5; i++) {
        int type = (i % 2 == 0) ? 1 : 3;
        physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { startX + i * 60.0f, 265.0f, 60.0f, 57.0f }, .originalY = 265.0f, .typeBlock = type };
    }

    // Plataforma acima
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { 700.0f, 100.0f, 60.0f, 57.0f }, .originalY = 100.0f, .typeBlock = 3 };

    // Plataformas lado a lado (grupo 2)
    startX = 1252.0f;
    for (int i = 0; i < 3; i++) {
        int type = (i % 2 == 0) ? 1 : 3;
        physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { startX + i * 60.0f, 215.0f, 60.0f, 57.0f }, .originalY = 215.0f, .typeBlock = type };
    }

    // Plataformas lado a lado (grupo 3)
    startX = 2006.0f;
    for (int i = 0; i < 5; i++) {
        int type = (i % 2 == 0) ? 1 : 3;
        physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { startX + i * 60.0f, 215.0f, 60.0f, 57.0f }, .originalY = 215.0f, .typeBlock = type };
    }

    // Plataformas lado a lado (grupo 4)
    startX = 2507.0f;
    for (int i = 0; i < 4; i++) {
        physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { startX + i * 60.0f, 215.0f, 60.0f, 57.0f }, .originalY = 215.0f, .typeBlock = 1 };
    }

    // Túneis
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { 1168.0f, 368.0f, 75.0f, 75.0f }, .originalY = 372.0f, .typeBlock = 2 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { 1912.0f, 368.0f, 75.0f, 75.0f }, .originalY = 372.0f, .typeBlock = 2 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { 1455.0f, 328.0f, 75.0f, 115.0f }, .originalY = 332.0f, .typeBlock = 2 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = { 2320.0f, 328.0f, 75.0f, 115.0f }, .originalY = 332.0f, .typeBlock = 2 };

    // Torre 1 (1695)
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {1695, 387, 60, 57}, .originalY = 387, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {1695, 327, 60, 57}, .originalY = 327, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {1695, 267, 60, 57}, .originalY = 267, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };

    // Torre 2 (1695 + 60 = 1755)
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {1755, 387, 60, 57}, .originalY = 387, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {1755, 327, 60, 57}, .originalY = 327, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };

    // Torre 3 (1755 + 60 = 1815)
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {1815, 387, 60, 57}, .originalY = 387, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };

    // Torre 4 (2711)
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2711, 387, 60, 57}, .originalY = 387, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };

    // Torre 5 (2711 + 60 = 2771)
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2771, 387, 60, 57}, .originalY = 387, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2771, 327, 60, 57}, .originalY = 327, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };

    // Torre 6 (2771 + 60 = 2831)
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2831, 387, 60, 57}, .originalY = 387, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2831, 327, 60, 57}, .originalY = 327, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2831, 267, 60, 57}, .originalY = 267, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };

    // Torre 7 (2831 + 60 = 2891)
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2891, 387, 60, 57}, .originalY = 387, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2891, 327, 60, 57}, .originalY = 327, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2891, 267, 60, 57}, .originalY = 267, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2891, 207, 60, 57}, .originalY = 207, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };

    // Torre 8 (2891 + 60 = 2951)
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2951, 387, 60, 57}, .originalY = 387, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2951, 327, 60, 57}, .originalY = 327, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2951, 267, 60, 57}, .originalY = 267, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2951, 207, 60, 57}, .originalY = 207, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {2951, 147, 60, 57}, .originalY = 147, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };

    // Torre 9 (Novo bloco isolado)
    physPlatforms[physPlatCount++] = (PhysPlatform_t){ .rect = {3145, 387, 60, 57}, .originalY = 387, .bounceOffset=0, .bouncing=false, .bounceDir=0, .bounceSpeed=0, .typeBlock=5 };

}



void UpdatePlatforms(PhysPlatform_t *physPlatform){

    for (int i = 4; i < physPlatCount; i++) { // Atualiza bounce
        if (physPlatform[i].bouncing) {
            if (physPlatform[i].bounceDir == 1) {
                physPlatform[i].bounceOffset -= 200.0f * GetFrameTime();
                if (physPlatform[i].bounceOffset <= -15.0f) {
                    physPlatform[i].bounceOffset = -15.0f;
                    physPlatform[i].bounceDir = -1;
                }
            } else {
                physPlatform[i].bounceOffset += 200.0f * GetFrameTime();
                if (physPlatform[i].bounceOffset >= 0.0f) {
                    physPlatform[i].bounceOffset = 0.0f;
                    physPlatform[i].bouncing = false;
                }
            }
            physPlatform[i].rect.y = physPlatform[i].originalY + physPlatform[i].bounceOffset;
        }
    }
}


void DrawBlocks(PhysPlatform_t *physPlatforms, Texture2D block1, Texture2D block3, Texture2D tunnels, Texture2D block5) {
   
    for (int i = 4; i < physPlatCount; i++) { // Desenha plataformas dinâmicas
         //Forma original que tava antes
         if(physPlatforms[i].typeBlock == 1) { // Se for do tipo block1
            Rectangle blockSource = { 0.0f, 0.0f, 16.0f, 16.0f }; // Sprite bloco
             DrawTexturePro(
             block1,
             blockSource,
             physPlatforms[i].rect,
             (Vector2){ 0.0f, 0.0f },
             0.0f,
             WHITE
         );

         }else if(physPlatforms[i].typeBlock == 2) { // Se for do tipo tunnel
            Rectangle blockSource = { 0.0f, 0.0f, 32.0f, 75.0f }; // Sprite túnel
             DrawTexturePro(
             tunnels,
             blockSource,
             physPlatforms[i].rect,
             (Vector2){ 0.0f, 0.0f },
             0.0f,
             WHITE
         );

         }else if(physPlatforms[i].typeBlock == 3) { // Se for do tipo block1
            Rectangle blockSource = { 1.0f, 0.0f, 15.0f, 16.0f }; // Sprite bloco
             DrawTexturePro(
             block3,
             blockSource,
             physPlatforms[i].rect,
             (Vector2){ 0.0f, 0.0f },
             0.0f,
             WHITE
         );
        

        }else if(physPlatforms[i].typeBlock == 5) { // Se for do tipo block1
            Rectangle blockSource = { 1.0f, 0.0f, 15.0f, 14.0f }; // Sprite bloco
             DrawTexturePro(
             block5,
             blockSource,
             physPlatforms[i].rect,
             (Vector2){ 0.0f, 0.0f },
             0.0f,
             WHITE
         );
        }
    }
}
