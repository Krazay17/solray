#pragma once

#include <stdint.h>
#include "raylib.h"

typedef enum
{
    A_NONE = 0,
    A_JUMP = (1 << 0),   // 1 (Binary: 00000001)
    A_ATTACK = (1 << 1), // 2 (Binary: 00000010)
    A_CROUCH = (1 << 2), // 4 (Binary: 00000100)
} Actions;

typedef struct
{
    Vector3 wishdir;
    uint8_t actionMask;
} Input;

void Update_Input(Input *inputs, int entities, int localId, Camera3D cam);
void Local_Input(Input *input, int localId, Camera3D cam);
void Remote_Input(Input *input, int id, float yaw);