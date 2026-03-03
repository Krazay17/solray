#ifndef APP_H
#define APP_H

#include "raylib.h"
#include <stdint.h>

typedef struct World World;
typedef struct SolConfig SolConfig;
extern Camera3D globalCamera;
extern bool AppShouldClose;

void SwitchWorld(World *world);
void main_loop(void);
void run(void);

void Sol_LocalInit(SolConfig *config);

#endif