#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "raylib.h"
#include "mario.h"
#include "platform.h"
#include "inimigos.h"

void CheckMarioHitboxY(Mario_t *Mario, PhysPlatform_t *physPlatforms, bool *isOnGround, Sound bumpSound);
void CheckMarioHitboxX(Mario_t *Mario, PhysPlatform_t *physPlatforms);
void CheckGoombaCollisionX(Mario_t *Mario, Goomba goomba);
void CheckPlantCollision(Mario_t *Mario, PlantaCarnivora planta);
void CheckTartarugaCollisionX(Mario_t *Mario, Tartaruga tartaruga);

#endif
