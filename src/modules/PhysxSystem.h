#pragma once
#include "raylib.h"

typedef struct World World;

typedef struct
{
    Vector3 position;
    Vector3 velocity;
    float frictionG, frictionA;
    float accelG, accelA;
    float speedG, speedA;
    float mass;
} Body;

void Update_Physx(Body *bodies, int entities, float dt);