#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include "mario.h"

void InitCamera(Camera2D *gameCamera, int width, int height);
void MoveCamera(Camera2D *gameCamera, Mario_t Mario, int width, int height);

#endif 
