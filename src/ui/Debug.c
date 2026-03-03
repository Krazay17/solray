#include "Debug.h"
#include <stdio.h>
#include "raylib.h"

Debug debugs[DEBUG_COUNT] = {
    {.text = "Framerate"},
    {.text = "Local Id"},
    {.text = "Entities"},
    {.text = "netLocalId"},
};

void Update_Debug(Debug *d, int count)
{
    for (int i = 0; i < count; i++)
    {
        Debug debug = d[i];
    }
}

void Draw_Debug(Debug *d, int count)
{

    DrawRectangle(0, 0, 150, 100, DARKPURPLE);

    for (int i = 0; i < count; i++)
    {
        Debug debug = d[i];
        char finalText[64];
        snprintf(finalText, sizeof(finalText), "%s: %d", debug.text, debug.value);
        DrawText(finalText, 0, i * 16, 16, GREEN);
    }
}
