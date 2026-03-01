#include "App.h"
#include <stdio.h>
#include <stdlib.h>
#include "Loader.h"
#include "net/Net.h"
#include "GlobalState.h"

// Global definitions
Scene *currentScene = NULL;
Camera3D globalCamera = {0};

// Factory forward declarations
Scene *CreateMenuScene();

static float NetAccumulator = 0.0f;
float MasterVolume = 1;

void SwitchScene(Scene *newScene)
{
    if (!newScene)
        return;
    if (currentScene)
    {
        currentScene->Unload(currentScene);
        free(currentScene);
    }
    currentScene = newScene;
    currentScene->Init(currentScene);
}

int main_loop(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "SolRay");
    InitAudioDevice();
    SetTargetFPS(2000);
    sol_init_loader();

    if (NetInit())
        NetConnect("127.0.0.1", 7777);

    globalCamera.position = (Vector3){10, 10, 10};
    globalCamera.up = (Vector3){0, 1, 0};
    globalCamera.fovy = 45;
    globalCamera.projection = CAMERA_PERSPECTIVE;

    SwitchScene(CreateMenuScene());

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        NetService();

        NetAccumulator += dt;
        if (NetAccumulator > 1.0f / 30.0f)
        {
            NetSendLocalPos(globalCamera.position.x, globalCamera.position.y, globalCamera.position.z);
            NetAccumulator = 0.0f;
        }

        if (currentScene)
            currentScene->Update(currentScene, dt);

        BeginDrawing();
        ClearBackground(DARKGRAY);
        if (currentScene)
            currentScene->Draw(currentScene);
        int fps = GetFPS();
        char buffer[10];
        sprintf(buffer, "%d", fps);
        DrawText(buffer, 0, 100, 24, GREEN);
        EndDrawing();
    }

    if (currentScene)
    {
        currentScene->Unload(currentScene);
        free(currentScene);
    }

    NetDeinit();
    CloseWindow();
    return 0;
}