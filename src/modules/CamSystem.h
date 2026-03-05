#pragma once

#include "raylib.h"
#include "PhysxSystem.h"

typedef struct
{
    float yaw, pitch;
    float sens;
} CamControl;

void Cam_Update(Camera3D *cam, Body *body, CamControl *ctrl);