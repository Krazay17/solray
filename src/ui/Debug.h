#pragma once

typedef struct Debug
{
    char text[100];
    int value;
} Debug;

typedef enum 
{
    DEBUG_FRAMERATE,
    DEBUG_LOCALID,
    DEBUG_ENTITIES,
    DEBUG_NETID,
    DEBUG_COUNT,
} DebugId;

extern Debug debugs[];

void Update_Debug(Debug *debugs, int count);
void Draw_Debug(Debug *debugs, int count);