#include "Debug.h"
#include <stdio.h>
#include "raylib.h"
#include "string.h"

static Debug debugs[MAX_DEBUGS] = {0};
static int count = 0;

void Debug_Log(const char *name, int value)
{
    for (int i = 0; i < count; i++)
    {
        // Does debugs have element with name already
        if (strcmp(debugs[i].name, name) == 0)
        {
            // If we have name element already, update the value and active true
            debugs[i].value = value;
            debugs[i].active = 1;
            // return because we are just updating one element
            return;
        }
    }
    if (count < MAX_DEBUGS)
    {
        snprintf(debugs[count].name, sizeof(debugs[count].name), "%s", name);
        debugs[count].value = value;
        debugs[count].active = 1;
        count++;
    }
}

void Debug_Draw(void)
{
    int padding = 10;
    int fontSize = 20;

    DrawRectangle(5, 5, 200, (count * (fontSize + 2)) + padding, Fade(BLACK, 0.5f));

    for (int i = 0; i < count; i++)
    {
        if (!debugs[i].active)
            continue;
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%s: %d", debugs[i].name, debugs[i].value);
        DrawText(buffer, 15, 10 + (i * (fontSize + 2)), fontSize, GREEN);
    }
}

void Debug_Clear()
{
    for (int i = 0; i < count; i++)
    {
        debugs[i].active = 0;
    }
}
