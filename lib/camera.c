#include "raylib.h"
#include <math.h>
#include "camera.h"
#include "mario.h"
#include "scene.h"

void InitCamera(Camera2D *gameCamera, int width, int height){
    gameCamera->offset = (Vector2){ width / 2.0f, height / 2.0f }; // Offset da câmera
    gameCamera->zoom   = 1.0f; // Zoom
    gameCamera->rotation = 0.0f; // Rotação
}

void MoveCamera(Camera2D *gameCamera, Mario_t *Mario, GameScene *scene, int width, int height){
    float old_position = gameCamera->target.x;
    float camTargetX = roundf(Mario->position.x); // Arredonda X da câmera

    // Impede o retorno de camera ao avancar na fase (quando nao ta na fase secreta)
    if(camTargetX < old_position && !Mario->stats.isOnCave){
        camTargetX = old_position;
    }

    gameCamera->target = (Vector2){ camTargetX, 300.0f }; // Atualiza alvo da câmera
    
    // Limita câmera à esquerda
    if (gameCamera->target.x < width / 2.0f){
        gameCamera->target.x = width / 2.0f; 
    }

    // Lmita a camera a direita
    if(gameCamera->target.x >= scene->scaledBackgroundWidth - (width/2.0f)){
        gameCamera->target.x = scene->scaledBackgroundWidth - (width/2.0f);
    }

    float currentCameraLeftEdge = gameCamera->target.x - gameCamera->offset.x + 3*Mario->assets.activeSprite->frameWidthCut;
    if (Mario->position.x < currentCameraLeftEdge) {
        Mario->position.x = currentCameraLeftEdge;
    }
}
