#ifndef APP_H
#define APP_H

#include "raylib.h"
#include <stdint.h>


#pragma pack(push, 1) // Tells the compiler: "No padding, keep it tight"
typedef struct {
    uint32_t playerID;
    float x, y, z;
    uint8_t inputState;
} PlayerPacket;
#pragma pack(pop) // Restore normal alignment for the rest of your code

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