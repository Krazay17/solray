#pragma once
#include "raylib.h"

typedef struct
{
    Sound pistolFire;
    Sound hitSound;
    Sound digi;
    Sound beep1;
    Sound beep2;
    Sound woong1;
    Sound menuMusic;
} AudioBank;

typedef struct
{
    AudioBank audio;
} ResourceManager;

ResourceManager *GetRM(void);

void sol_init_loader(void);
Sound LoadSoundFromRes(const char *id, const char *ext);