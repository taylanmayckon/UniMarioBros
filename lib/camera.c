#include "raylib.h"
#include "camera.h"
#include "mario.h"

void InitCamera(Camera2D *gameCamera, int width, int height){
    gameCamera->offset = (Vector2){ width / 2.0f, height / 2.0f }; // Offset da câmera
    gameCamera->zoom   = 1.0f; // Zoom
    gameCamera->rotation = 0.0f; // Rotação
}

void MoveCamera(Camera2D *gameCamera, Mario_t Mario, int width, int height){
    float camTargetX = roundf(Mario.position.x); // Arredonda X da câmera
    gameCamera->target = (Vector2){ camTargetX, 290.0f }; // Atualiza alvo da câmera
    
    // Limita câmera à esquerda
    if (gameCamera->target.x < width / 2.0f){
        gameCamera->target.x = width / 2.0f; 
    }
}