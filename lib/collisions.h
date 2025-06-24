#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "raylib.h"
#include "mario.h"
#include "platform.h"
#include "inimigos.h"

void CheckMarioHitboxY(Mario_t *Mario, PhysPlatform_t *physPlatforms, bool *isOnGround, Sound bumpSound);
void CheckMarioHitboxX(Mario_t *Mario, PhysPlatform_t *physPlatforms);
void CheckEnemyCollision(Mario_t *Mario, Rectangle enemyRect);
void CheckFlagEndCollision(Mario_t *Mario, PhysPlatform_t *physPlatforms);

#endif
