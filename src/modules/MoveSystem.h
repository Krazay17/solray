#pragma once

#include "InputSystem.h"
#include "PhysxSystem.h"

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

void Update_Move(Input *inputs, Body *bodies, int entities, float dt);