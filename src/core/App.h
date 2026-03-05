#ifndef APP_H
#define APP_H

#include "raylib.h"
#include <stdint.h>

typedef struct World World;
typedef struct SolConfig SolConfig;

typedef enum WorldId
{
    WORLD_GAME,
    WORLD_UI,
    WORLD_MENU,
    WORLD_COUNT,
} WorldId;

typedef struct
{
    World *worlds[WORLD_COUNT];
    int width;
    int height;
} EngineState;

extern bool AppShouldClose;
extern EngineState engineState;

void ChangeMenu(World *newMenu);
void OpenWorld(WorldId w);
void main_loop(void);
void run(void);

void Sol_LocalInit(SolConfig *config);
static void InitWorlds();
void SyncWindowSize();

#endif