#pragma once
#include <stdbool.h>

#define MAX_DEBUGS 24

typedef struct Debug
{
    char name[32];
    int value;
    bool active;
} Debug;

void Debug_Log(const char *name, int value);
void Debug_Draw();
void Debug_Clear();