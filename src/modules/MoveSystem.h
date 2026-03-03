#pragma once

#include "InputSystem.h"
#include "PhysxSystem.h"

typedef struct Entities Entities;

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

void Update_Move(Input *inputs, Body *bodies, Entities *entities, float dt);