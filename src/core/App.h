#ifndef APP_H
#define APP_H

#include "raylib.h"

// The "Interface" definition
typedef struct Scene Scene;

typedef void (*SceneInitCallback)(Scene *self);
typedef void (*SceneUpdateCallback)(Scene *self, float delta);
typedef void (*SceneDrawCallback)(Scene *self);
typedef void (*SceneUnloadCallback)(Scene *self);

struct Scene
{
    SceneInitCallback Init;
    SceneUpdateCallback Update;
    SceneDrawCallback Draw;
    SceneUnloadCallback Unload;
    void *state;
};

int main_loop(void);

// Global Scene Manager access
void SwitchScene(Scene *newScene);

// Global Camera access (Shared across 3D scenes)
extern Camera3D globalCamera;

#endif