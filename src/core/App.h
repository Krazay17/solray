#ifndef APP_H
#define APP_H

#include "raylib.h"
#include <stdint.h>

#pragma pack(push, 1) // Tells the compiler: "No padding, keep it tight"
typedef struct
{
    uint32_t playerID;
    float x, y, z;
    uint8_t inputState;
} PlayerPacket;
#pragma pack(pop) // Restore normal alignment for the rest of your code

typedef struct World World;
extern Camera3D globalCamera;

int main_loop(void);
void SwitchWorld(World *world);

#endif