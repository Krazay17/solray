#include "InputSystem.h"
#include "raymath.h"

void Update_Input(Input *inputs, int entities, int localId, Camera3D cam)
{
    for (int i = 0; i < entities; i++)
    {
        Input *input = &inputs[i];
        if (i == localId)
            Local_Input(input, localId, cam);
    }
}

void Local_Input(Input *input, int localId, Camera3D cam)
{
    Vector3 fwd = Vector3Subtract(cam.target, cam.position);
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

void Remote_Input(Input *input, int id, float yaw)
{
}
