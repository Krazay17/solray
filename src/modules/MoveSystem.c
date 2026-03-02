#include "MoveSystem.h"
#include "raymath.h"

void Update_Move(Input *inputs, Body *bodies, int entities, float dt)
{
    for (int i = 0; i < entities; i++)
    {
        Input *input = &inputs[i];
        Body *body = &bodies[i];

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