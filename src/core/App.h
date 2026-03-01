#ifndef APP_H
#define APP_H

#include "raylib.h"
#include <stdint.h>

typedef struct World World;

#pragma pack(push, 1) // Tells the compiler: "No padding, keep it tight"
typedef struct
{
    uint32_t playerID;
    float x, y, z;
    uint8_t inputState;
} PlayerPacket;
#pragma pack(pop) // Restore normal alignment for the rest of your code

int main_loop(void);

// Global World Manager access
void SwitchWorld(World *world);

// Global Camera access (Shared across 3D scenes)
extern Camera3D globalCamera;

#endif