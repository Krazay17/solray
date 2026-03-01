#include "Button.h"

bool UpdateButton(Button *btn)
{
    Vector2 mouse = GetMousePosition();
    btn->wasHovered = btn->isHovered;
    btn->isHovered = CheckCollisionPointRec(mouse, btn->rect);

    if (btn->isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true; // The button was clicked
    }
    return false;
}

void DrawButton(Button *btn)
{
    Color drawColor = btn->isHovered ? SKYBLUE : btn->baseColor;

    DrawRectangleRec(btn->rect, drawColor);
    // Add a simple border if hovered
    if (btn->isHovered)
        DrawRectangleLinesEx(btn->rect, 2, BLUE);

    // Center text logic
    int fontSize = 20;
    int textWidth = MeasureText(btn->text, fontSize);
    DrawText(btn->text,
             btn->rect.x + (btn->rect.width / 2) - (textWidth / 2),
             btn->rect.y + (btn->rect.height / 2) - (fontSize / 2),
             fontSize, WHITE);
}

bool UpdateSlider(Slider *slider)
{
    float lastValue = slider->value;
    Vector2 mouse = GetMousePosition();
    slider->wasHovered = slider->isHovered;
    slider->isHovered = CheckCollisionPointRec(mouse, slider->bg);

    if (slider->isHovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        float localX = mouse.x - slider->bg.x;
        float newValue = localX / slider->bg.width;
        if (newValue < 0.0f)
            newValue = 0.0f;
        if (newValue > 1.0f)
            newValue = 1.0f;
        if (newValue != lastValue)
        {
            slider->value = newValue;
            return true;
        }
    }
    return false;
}

void DrawSlider(Slider *slider)
{
    Color drawColor = slider->isHovered ? SKYBLUE : slider->bgC;
    DrawRectangleRec(slider->bg, drawColor);
    if (slider->isHovered)
        DrawRectangleLinesEx(slider->bg, 2.0f, BLUE);
    DrawRectangleRec((Rectangle){
                         slider->bg.x,
                         slider->bg.y,
                         slider->bg.width * slider->value,
                         slider->bg.height,
                     },
                     slider->fillC);

    DrawRectangleRec((Rectangle){
                         .x = slider->bg.x + (slider->bg.width * slider->value) - slider->handleWidth / 2.0f,
                         slider->bg.y,
                         slider->handleWidth,
                         slider->bg.height,
                     },
                     slider->handleC);
    // Center text logic
    int fontSize = 20;
    int textWidth = MeasureText(slider->text, fontSize);
    DrawText(slider->text,
             slider->bg.x + (slider->bg.width / 2) - (textWidth / 2),
             slider->bg.y + (slider->bg.height / 2) - (fontSize / 2),
             fontSize, WHITE);
}
