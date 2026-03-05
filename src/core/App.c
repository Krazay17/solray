#include "App.h"
#include "World.h"
#include <stdio.h>
#include <stdlib.h>
#include "LocalConfig.h"
#include "Loader.h"
#include "net/Net.h"
#include "GlobalState.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Global definitions
float TimeStep = 1.0f / 60.0f;
bool AppShouldClose = false;
SolConfig LocalConfig = {0};

static World *persistentWorld = NULL;
static World *currentWorld = NULL;
static World *pendingWorld = NULL;
static float Accumulator = 0.0f;

void SwitchWorld(World *world)
{
    if (pendingWorld)
    {
        pendingWorld->Exit(pendingWorld);
        free(pendingWorld);
    }
    pendingWorld = world;
}

static void PerformSwitchWorld()
{
    if (!pendingWorld)
        return;
    if (currentWorld)
    {
        currentWorld->Exit(currentWorld);
        if (!currentWorld->staticFlag)
        {
            currentWorld->Kill(currentWorld);
            free(currentWorld);
        }
        currentWorld = NULL;
    }
    currentWorld = pendingWorld;
    pendingWorld = NULL;

    if (currentWorld && !currentWorld->initialized)
    {
        currentWorld->Init(currentWorld);
        currentWorld->initialized = 1;
    }
    if (currentWorld && currentWorld->Open)
        currentWorld->Open(currentWorld);
}

static void Cleanup()
{
    if (currentWorld)
    {
        if (currentWorld->Kill)
            currentWorld->Kill(currentWorld);
        if (!currentWorld->staticFlag)
            free(currentWorld);
        currentWorld = NULL;
    }

    NetDeinit();
    CloseLoader();
    CloseAudioDevice();
    CloseWindow();
}

void main_loop(void)
{
    PerformSwitchWorld();
    World *w = currentWorld;
    World *p = persistentWorld;
    if (!w)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("No World Loaded", 10, 10, 20, RED);
        EndDrawing();
        return;
    }

    float dt = GetFrameTime();
    NetService();

    Accumulator += dt;
    while (Accumulator > TimeStep)
    {
        w->Step(w, TimeStep);
        Accumulator -= TimeStep;
    }

    w->Tick(w, dt);

    BeginDrawing();
    ClearBackground(DARKGRAY);
    w->Draw(w);
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
    InitWindow(1280, 720, "SolRay");
    SetExitKey(0);
    InitAudioDevice();
    SetTargetFPS(2000);
    sol_init_loader();
    LocalConfig = Load_Config();
    Sol_LocalInit(&LocalConfig);

    if (NetInit())
        NetConnect("answer-cuba.gl.at.ply.gg", 35101);

    persistentWorld = GetSettingsWorld();
    SwitchWorld(GetMenuWorld());

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
