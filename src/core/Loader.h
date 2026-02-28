#pragma once
#include "raylib.h"

typedef struct
{
    Sound flashAway;
    Sound pistolFire;
    Sound hitSound;
} AudioBank;

typedef struct
{
    AudioBank audio;
} ResourceManager;

ResourceManager *GetRM(void);

void sol_init_loader(void);