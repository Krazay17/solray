#include "PhysxSystem.h"
#include "raymath.h"

void Update_Physx(Body *bodies, int entities, float dt)
{
    for (int i = 0; i < entities; i++)
    {
        Body *body = &bodies[i];
        if (body->mass > 0)
            body->velocity.y -= 9.81f * dt;
        body->position = Vector3Add(body->position, Vector3Scale(body->velocity, dt));

        float friction = (body->position.y <= 0.0f) ? body->frictionG : body->frictionA;
        body->velocity = Vector3Scale(body->velocity, 1.0f - (friction * dt));

        // Floor Snap (Crude Collision)
        if (body->position.y < 0.0f)
        {
            body->position.y = 0.0f;
            body->velocity.y = 0.0f;
        }
    }
}
