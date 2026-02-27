#include "core/App.h"
#include <stdio.h>
#include <stdlib.h>

// Global definitions
Scene *currentScene = NULL;
Camera3D globalCamera = {0};

// Factory forward declarations
Scene *CreateMenuScene();

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

// --- MAIN ENGINE LOOP ---

int main(void)
{
    InitWindow(800, 450, "SolRay");
    InitAudioDevice();

    globalCamera.position = (Vector3){10, 10, 10};
    globalCamera.up = (Vector3){0, 1, 0};
    globalCamera.fovy = 45;
    globalCamera.projection = CAMERA_PERSPECTIVE;

    SwitchScene(CreateMenuScene());
    SetTargetFPS(1000);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (currentScene)
            currentScene->Update(currentScene, dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);
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
    CloseWindow();
    return 0;
}