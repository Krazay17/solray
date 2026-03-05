#pragma once
#include <stdbool.h>

typedef struct World World;
typedef void (*WorldInit)(World *self);
typedef void (*WorldOpen)(World *self);
typedef bool (*WorldPoll)(World *self, float dt);
typedef void (*WorldStep)(World *self, float dt);
typedef void (*WorldTick)(World *self, float dt);
typedef void (*WorldDraw)(World *self);
typedef void (*WorldExit)(World *self);
typedef void (*WorldKill)(World *self);

struct World
{
    WorldInit Init;
    WorldOpen Open;
    WorldPoll Poll;
    WorldStep Step;
    WorldTick Tick;
    WorldDraw Draw;
    WorldExit Exit;
    WorldKill Kill;
    bool active;
    bool staticFlag;
    bool initialized;
    void *state;
};

World *GetMenuWorld();
World *GetGameWorld();
World *GetSettingsWorld();
World *GetPMenuWorld();