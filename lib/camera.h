#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include "mario.h"
#include "scene.h"

void InitCamera(Camera2D *gameCamera, int width, int height);
void MoveCamera(Camera2D *gameCamera, Mario_t *Mario, GameScene *scene, int width, int height, bool end_screen);

#endif 
