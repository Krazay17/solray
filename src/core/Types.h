#pragma once
#include <stdint.h>
#include "raylib.h"

#define MAX_ANIMS 12

typedef struct Entities Entities;

typedef enum
{
    ANIM_IDLE,
    ANIM_FWD,
    ANIM_ATTACK1,
} AnimId;

typedef struct Sol_Anim
{
    Model *model;
    ModelAnimation anims[MAX_ANIMS];
    AnimId currentAnim;
    float animTime;
} Sol_Anim;

typedef struct
{
    float yaw, pitch;
    float sens;
} CamControl;

typedef enum
{
    A_NONE = 0,
    A_JUMP = (1 << 0),   // 1 (Binary: 00000001)
    A_ATTACK = (1 << 1), // 2 (Binary: 00000010)
    A_CROUCH = (1 << 2), // 4 (Binary: 00000100)
} Actions;

typedef struct Sol_Input
{
    float yaw,pitch;
    float lookSens;
    Vector3 lookDir;
    Vector3 wishdir;
    uint8_t actionMask;
} Sol_Input;

typedef struct Sol_View
{
    Model model;
    float yOffset;
    float yawOffset;
} Sol_View;

typedef struct Body
{
    Vector3 position;
    Vector3 velocity;
    Vector3 rotation;
    float yaw, pitch;
    float frictionG, frictionA;
    float accelG, accelA;
    float speedG, speedA;
    float mass;
} Body;

typedef enum
{
    walk,
    fall,
} MoveState;

typedef struct
{
    MoveState currentState;
    MoveState lastState;
} Move;