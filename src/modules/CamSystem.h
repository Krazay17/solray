#pragma once

#include "raylib.h"
#include "PhysxSystem.h"

typedef struct
{
    float yaw, pitch;
    float sens;
} CamControl;

void Update_Cam(Camera3D *cam, Body *body, CamControl *ctrl);