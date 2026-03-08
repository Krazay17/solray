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
    Shader light;
} ShaderBank;

typedef struct
{
    Material defMat;

    Mesh cylinderMesh;
    Model cylinderModel;

    Model wizardModel;
    ModelAnimation *wizardAnims;
    int wizardAnimCount;

} ModelBank;

typedef struct
{
    AudioBank audio;
    ShaderBank shaders;
    ModelBank models;
} ResourceManager;

ResourceManager *GetRM(void);

void sol_init_loader(void);
Shader LoadShaderFromRes(const char* vsId, const char* fsId);
Sound LoadSoundFromRes(const char *id, const char *ext);
Model LoadModelFromRes(const char *id, const char *ext, ModelAnimation **anims, int *animCount);
void CloseLoader(void);