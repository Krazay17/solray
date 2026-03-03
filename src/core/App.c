#include "App.h"
#include "apps/World.h"
#include <stdio.h>
#include <stdlib.h>
#include "LocalConfig.h"
#include "Loader.h"
#include "net/Net.h"
#include "GlobalState.h"

// Global definitions
World *currentWorld = NULL;
static World *pendingWorld = NULL;
Camera3D globalCamera = {0};
bool AppShouldClose = false;

SolConfig LocalConfig = {0};

// Factory forward declarations
World *CreateMenuWorld();

static float Accumulator = 0.0f;
float TimeStep = 1.0f / 60.0f;

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
            free(currentWorld);
        currentWorld = NULL;
    }
    currentWorld = pendingWorld;
    pendingWorld = NULL;
    if (currentWorld && currentWorld->Init)
        currentWorld->Init(currentWorld);
}

void main_loop(void)
{
    PerformSwitchWorld();
    World *w = currentWorld;
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
}

void run()
{

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "SolRay");
    SetExitKey(0);
    InitAudioDevice();
    SetTargetFPS(2000);
    sol_init_loader();
    LocalConfig = Load_Config();
    Sol_LocalInit(&LocalConfig);

    if (NetInit())
        NetConnect("answer-cuba.gl.at.ply.gg", 35101);

    globalCamera.position = (Vector3){10, 10, 10};
    globalCamera.up = (Vector3){0, 1, 0};
    globalCamera.fovy = 45;
    globalCamera.projection = CAMERA_PERSPECTIVE;

    SwitchWorld(CreateMenuWorld());

    while (!AppShouldClose && !WindowShouldClose())
    {
        main_loop();
    }

    if (currentWorld)
    {
        currentWorld->Exit(currentWorld);
        if (!currentWorld->staticFlag)
            free(currentWorld);
        currentWorld = NULL;
    }

    NetDeinit();
    CloseLoader();
    CloseAudioDevice();
    CloseWindow();
}

void Sol_LocalInit(SolConfig *config)
{
    SetMasterVolume(config->volume);
    SetWindowOpacity(config->opacity);
}
