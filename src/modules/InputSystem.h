#pragma once
#include "raylib.h"
#include "core/Types.h"

void Input_Update(
    Sol_Input *inputs,
    Entities *entities,
    int localId,
    Camera3D *cam,
    CamControl *ctrl);