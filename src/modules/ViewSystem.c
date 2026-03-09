#include "ViewSystem.h"
#include "core/GameWorld.h"
#include "PhysxSystem.h"

void View_Update(Sol_View *views, Body *bodies, Entities *entities)
{
    for (int i = 0; i < entities->count; i++)
    {
        if (!entities->active[i])
            continue;

        DrawModelEx(
            views[i].model,
            bodies[i].position,
            (Vector3){0, 1, 0},
            -bodies[i].yaw + views[i].yawOffset,
            (Vector3){1, 1, 1},
            BLANK);
    }
}