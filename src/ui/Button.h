#pragma once
#include <raylib.h>

typedef struct
{
    Rectangle rect;
    Color baseColor;
    bool isHovered;
    bool isPressed;
} Button;

bool UpdateButton(Button *button);
void DrawButton(Button *btn, const char *text);