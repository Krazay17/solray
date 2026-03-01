#pragma once

typedef struct World World;
typedef void (*WorldInit)(World *self);
typedef void (*WorldStep)(World *self, float dt);
typedef void (*WorldTick)(World *self, float dt);
typedef void (*WorldDraw)(World *self);
typedef void (*WorldExit)(World *self);

struct World
{
    WorldInit Init;
    WorldStep Step;
    WorldTick Tick;
    WorldDraw Draw;
    WorldExit Exit;
    void *state;
};