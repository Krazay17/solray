#pragma once
#include <raylib.h>
#include "utils/SolMath.h"

typedef struct
{
    union
    {
        Rectangle rect;
        Rect SolRect;
    };
    Color baseColor;
    const char *text;
    float clickAnim;
    float hoverAnim;
    float clickAnimSpeed;
    float hoverAnimSpeed;
    bool isHovered;
    bool wasHovered;
    bool isPressed;
    bool wasPressed;
} Button;

typedef struct
{
    union
    {
        Rectangle rect;
        Rect SolRect;
    };
    float handleWidth;
    Color bgC;
    Color fillC;
    Color handleC;
    float value;
    const char *text;
    bool isHovered;
    bool wasHovered;
    bool isPressed;
    bool wasPressed;
} Slider;

bool UpdateButton(Button *button, float dt);
void DrawButton(Button *btn);
void Button_Reset(Button *btn);

bool UpdateSlider(Slider *slider);
void DrawSlider(Slider *slider);