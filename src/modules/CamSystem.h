#pragma once
#include "raylib.h"
#include "core/Types.h"

void Cam_Update(Camera3D *cam, Body *body, CamControl *ctrl, bool locked);