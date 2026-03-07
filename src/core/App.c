#include "App.h"
#include "World.h"
#include <stdio.h>
#include <stdlib.h>
#include "LocalConfig.h"
#include "Loader.h"
#include "net/Net.h"
#include "GlobalState.h"
#include "modules/EventSystem.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Global definitions
float TimeStep = 1.0f / 60.0f;
bool AppShouldClose = false;
SolConfig LocalConfig = {0};

EngineState engineState = {0};

static World *currentWorld = NULL;
static World *pendingWorld = NULL;
static float Accumulator = 0.0f;

static void Cleanup()
{
    for (int i = 0; i < WORLD_COUNT; i++)
    {
        World *w = engineState.worlds[i];
        if (w && w->Kill)
            w->Kill(w);
    }

    NetDeinit();
    CloseLoader();
    CloseAudioDevice();
    CloseWindow();
}

void main_loop(void)
{
    float dt = GetFrameTime();
    NetService();
    if (IsKeyPressed(KEY_LEFT_ALT))
    {
        if (IsCursorHidden())
            ShowCursor();
    }

    int width = GetScreenWidth();
    int height = GetScreenHeight();
    if (width != engineState.width || height != engineState.height)
    {
        SyncWindowSize(width, height);
    }

    for (int i = WORLD_COUNT - 1; i >= 0; i--)
    {
        World *w = engineState.worlds[i];
        if (w && w->active && w->Poll)
        {
            if (w->Poll(w, dt))
                break;
        }
    }

    Accumulator += dt;
    while (Accumulator > TimeStep)
    {
        for (int i = 0; i < WORLD_COUNT; i++)
        {
            World *w = engineState.worlds[i];
            if (w && w->active && w->Step)
                w->Step(w, TimeStep);
        }
        Accumulator -= TimeStep;
    }

    for (int i = 0; i < WORLD_COUNT; i++)
    {
        World *w = engineState.worlds[i];
        if (w && w->active && w->Tick)
            w->Tick(w, dt);
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);
    for (int i = 0; i < WORLD_COUNT; i++)
    {
        World *w = engineState.worlds[i];
        if (w && w->active && w->Draw)
            w->Draw(w);
    }
    EndDrawing();

#ifdef __EMSCRIPTEN__
    if (AppShouldClose || WindowShouldClose())
    {
        emscripten_cancel_main_loop();
        Cleanup();
    }
#endif
}

void run()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_ALWAYS_RUN);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    engineState.width = 1280;
    engineState.height = 720;
    InitWindow(engineState.width, engineState.height, "SolRay");
    SetExitKey(0);
    InitAudioDevice();
    SetTargetFPS(2000);
    sol_init_loader();
    LocalConfig = Load_Config();
    Sol_LocalInit(&LocalConfig);

    if (NetInit())
        NetConnect("answer-cuba.gl.at.ply.gg", 35101);

    InitWorlds();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else

    while (!AppShouldClose && !WindowShouldClose())
    {
        main_loop();
    }
    Cleanup();

#endif
}

void Sol_LocalInit(SolConfig *config)
{
    SetMasterVolume(config->volume);
    SetWindowOpacity(config->opacity);
}

static void InitWorlds()
{
    World *menu = engineState.worlds[WORLD_MENU] = GetMenuWorld();
    if (!menu->initialized)
    {
        menu->Init(menu);
        menu->initialized = true;
    }

    World *game = engineState.worlds[WORLD_GAME] = GetGameWorld();
    if (!game->initialized)
    {
        game->Init(game);
        game->initialized = true;
    }
}

void SyncWindowSize(int width, int height)
{
    engineState.width = width;
    engineState.height = height;
    EmitEvent(EVENT_WINDOW_RESIZE, &(WindowData){width, height});
}

void OpenWorld(WorldId id)
{
    for (int i = 0; i < id; i++)
    {
        World *ew = engineState.worlds[i];
        if (ew && ew->active && ew->Exit)
            ew->Exit(ew);
    }
    World *w = engineState.worlds[id];
    if (w)
    {
        w->active = 1;
        if (w->Open)
            w->Open(w);
    }
}

void ChangeMenu(World *newMenu)
{
    World *oldMenu = engineState.worlds[WORLD_MENU];

    if (oldMenu && oldMenu->Exit)
    {
        oldMenu->Exit(oldMenu);
    }

    engineState.worlds[WORLD_MENU] = newMenu;

    if (newMenu)
    {
        if (!newMenu->initialized)
        {
            newMenu->Init(newMenu);
            newMenu->initialized = true;
        }
        if (newMenu->Open)
        {
            newMenu->Open(newMenu);
        }
        newMenu->active = true;
    }
}