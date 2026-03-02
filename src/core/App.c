#include "App.h"
#include "apps/World.h"
#include <stdio.h>
#include <stdlib.h>
#include "Loader.h"
#include "net/Net.h"
#include "GlobalState.h"

// Global definitions
World *currentWorld = NULL;
static World *pendingWorld = NULL;
Camera3D globalCamera = {0};
float WindowOpacity = 1.0f;

// Factory forward declarations
World *CreateMenuWorld();

static float Accumulator = 0.0f;
float MasterVolume = 1;
float TimeStep = 1.0f / 60.0f;

void SwitchWorld(World *world)
{
    if (pendingWorld)
    {
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
        free(currentWorld);
        currentWorld = NULL;
    }
    currentWorld = pendingWorld;
    pendingWorld = NULL;
    if (currentWorld && currentWorld->Init)
        currentWorld->Init(currentWorld);
}

int main_loop(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "SolRay");
    SetExitKey(0);
    InitAudioDevice();
    SetTargetFPS(2000);
    sol_init_loader();

    // if (NetInit())
    //     NetConnect("127.0.0.1", 7777);

    globalCamera.position = (Vector3){10, 10, 10};
    globalCamera.up = (Vector3){0, 1, 0};
    globalCamera.fovy = 45;
    globalCamera.projection = CAMERA_PERSPECTIVE;

    SwitchWorld(CreateMenuWorld());

    while (!WindowShouldClose())
    {
        PerformSwitchWorld();
        World *w = currentWorld;
        if (!w)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("No World Loaded", 10, 10, 20, RED);
            EndDrawing();
            continue; // Skip to next frame
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

        int fps = GetFPS();
        char buffer[10];
        sprintf(buffer, "%d", fps);

        BeginDrawing();
        ClearBackground(DARKGRAY);
        w->Draw(w);
        DrawText(buffer, 0, 100, 24, GREEN);
        EndDrawing();
    }

    if (currentWorld)
    {
        currentWorld->Exit(currentWorld);
        free(currentWorld);
    }

    CloseLoader();
    NetDeinit();
    CloseWindow();
    return 0;
}