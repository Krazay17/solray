#include "InputSystem.h"
#include "CamSystem.h"
#include "raymath.h"
#include "core/GameWorld.h"

static void Local_Input(Sol_Input *input, int localId, Camera3D *cam, CamControl *ctrl)
{
    input->yaw = ctrl->yaw;
    input->pitch = ctrl->pitch;
    Vector3 fwd = Vector3Subtract(cam->target, cam->position);
    fwd.y = 0;
    fwd = Vector3Normalize(fwd);
    Vector3 right = Vector3CrossProduct(fwd, (Vector3){0, 1, 0});

    Vector3 tempWish = {0};
    if (IsKeyDown(KEY_W))
        tempWish = Vector3Add(tempWish, fwd);
    if (IsKeyDown(KEY_S))
        tempWish = Vector3Subtract(tempWish, fwd);
    if (IsKeyDown(KEY_D))
        tempWish = Vector3Add(tempWish, right);
    if (IsKeyDown(KEY_A))
        tempWish = Vector3Subtract(tempWish, right);
    if (IsKeyPressed(KEY_SPACE))
        input->actionMask |= A_JUMP;

    input->wishdir = Vector3Normalize(tempWish);
}

static void Remote_Input(Sol_Input *input, int id, float yaw)
{
}

void Input_Update(Sol_Input *inputs, Entities *entities, int localId, Camera3D *cam, CamControl *ctrl)
{
    for (int i = 0; i < entities->count; i++)
    {
        if (!entities->active[i])
            continue;
        Sol_Input *input = &inputs[i];
        if (i == localId)
            Local_Input(input, localId, cam, ctrl);
    }
}
