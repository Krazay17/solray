#pragma once
#include "core/GameWorld.h"
#include "raylib.h"

#define MAX_ANIMS 12

typedef struct
{
    ModelSkeleton skele;
    ModelAnimation anims[MAX_ANIMS];
} EntityAnim;

void Anim_Update(EntityAnim *anims, Entities *entities, float dt);
