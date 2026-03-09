#include "MoveSystem.h"
#include "raymath.h"
#include "core/GameWorld.h"

void Move_Update(Sol_Input *inputs, Body *bodies, Entities *entities, float dt)
{
    for (int i = 0; i < entities->count; i++)
    {
        if (!entities->active[i])
            continue;
        Sol_Input *input = &inputs[i];
        Body *body = &bodies[i];
        body->yaw = input->yaw;
        Vector3RotateByAxisAngle(body->rotation, (Vector3){0,1,0}, input->yaw);

        // 1. Basic Movement
        // Note: Using Vector3Add here might make the player accelerate forever.
        // Usually, we want to move toward a target velocity.
        body->velocity.x += input->wishdir.x * body->speedG * dt;
        body->velocity.z += input->wishdir.z * body->speedG * dt;

        // 2. Jumping (Only if on or very near the floor)
        if ((input->actionMask & A_JUMP) && body->position.y <= 0.01f)
        {
            body->velocity.y = 5.0f;
        }

        // 3. Reset the mask so the jump doesn't repeat next frame
        input->actionMask = 0;
    }
}