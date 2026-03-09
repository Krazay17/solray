#include "InputSystem.h"
#include "raylib.h"
#include "CamSystem.h"
#include "raymath.h"
#include "core/GameWorld.h"
#include "core/GlobalState.h"

static void Local_Input(Sol_Input *input, int localId)
{
    if (IsCursorHidden())
    {
        // 1. Get Mouse Delta (How much did the mouse move this frame?)
        Vector2 mouseDelta = GetMouseDelta();

        // 2. Update Euler Angles
        input->yaw += mouseDelta.x * LocalConfig.lookSens;
        input->pitch -= mouseDelta.y * LocalConfig.lookSens;

        input->yaw = fmodf(input->yaw, 360.0f);

        // 3. Clamp Pitch (Prevent the camera from flipping upside down)
        if (input->pitch > 89.0f)
            input->pitch = 89.0f;
        if (input->pitch < -89.0f)
            input->pitch = -89.0f;
    }

    input->lookDir.x = cosf(DEG2RAD * input->yaw) * cosf(DEG2RAD * input->pitch);
    input->lookDir.y = sinf(DEG2RAD * input->pitch);
    input->lookDir.z = sinf(DEG2RAD * input->yaw) * cosf(DEG2RAD * input->pitch);
    Vector3 fwd = input->lookDir;
    fwd.y = 0;
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

void Input_Update(Sol_Input *inputs, Entities *entities, int localId)
{
    for (int i = 0; i < entities->count; i++)
    {
        if (!entities->active[i])
            continue;

        Sol_Input *input = &inputs[i];

        if (i == localId)
            Local_Input(input, localId);
    }
}
