#pragma once
#include "Types.h"
#include "net/Net.h"
#include "models/rlights.h"

#define MAX_ENTITIES 100
extern Camera3D globalCamera;

typedef struct Entities
{
    bool active[MAX_ENTITIES];
    int count;
} Entities;

typedef struct GameState
{
    Sound sound;
    Mesh knotMesh;
    Model knotModel;
    Model *wizardModel;
    Model *playerModel;
    CamControl camControl;

    Light sunlight;

    int localId;
    int netLocalId;
    int netToLocal[MAX_CLIENTS];

    Entities entities;
    Sol_Input inputs[MAX_ENTITIES];
    Body bodies[MAX_ENTITIES];
    Sol_Anim anims[MAX_ENTITIES];
    Sol_View views[MAX_ENTITIES];

} GameState;