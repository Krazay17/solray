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
    Material defMat;
    Mesh cylinderMesh;
    Model cylinderModel;
} DrawBank;


typedef struct
{
    AudioBank audio;
    DrawBank draw;
} ResourceManager;

ResourceManager *GetRM(void);

void sol_init_loader(void);
Sound LoadSoundFromRes(const char *id, const char *ext);
void CloseLoader(void);