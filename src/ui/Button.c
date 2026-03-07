#include "Button.h"
#include <math.h>
#include "raymath.h"

static float ButtonAnim(float dt, float value, float speed)
{
    speed = (speed > 0.0f) ? speed : 4.0f;
    value -= dt * speed;
    if (value < 0.0f)
        value = 0.0f;
    return value;
}

bool UpdateButton(Button *btn, float dt)
{
    if (btn->clickAnim > 0)
        btn->clickAnim = ButtonAnim(dt, btn->clickAnim, btn->clickAnimSpeed);

    Vector2 mouse = GetMousePosition();
    btn->wasHovered = btn->isHovered;
    // btn->isHovered = CheckCollisionPointRec(mouse, btn->rect);
    btn->isHovered = Sol_Check_2d_Collision(
        (Vec2){mouse.x, mouse.y},
        btn->SolRect);

    if (btn->isHovered)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            btn->clickAnim = 1.0f;
            return true; // The button was clicked
        }

        float speed = 5.0f;

        // Normalize sin to 0.0f - 1.0f
        float t = (sinf(GetTime() * speed) * 0.5f) + 0.5f;
        btn->hoverAnim = Lerp(0.33f, 0.66f, t);
    }
    else
    {
        btn->hoverAnim = ButtonAnim(dt, btn->hoverAnim, 8.0f);
    }
    return false;
}

void DrawButton(Button *btn)
{
    // Start with base
    Color drawColor = btn->baseColor;

    // Layer 1: Apply Hover Pulse (Glow white)
    if (btn->hoverAnim > 0)
    {
        drawColor = ColorLerp(drawColor, WHITE, btn->hoverAnim);
    }

    // Layer 2: Apply Click Flash (Flash black/white)
    if (btn->clickAnim > 0)
    {
        drawColor = ColorLerp(drawColor, BLACK, btn->clickAnim);
    }

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

void Button_Reset(Button *btn)
{
    btn->clickAnim = 0;
    btn->hoverAnim = 0;
}

bool UpdateSlider(Slider *slider)
{
    float lastValue = slider->value;
    Vector2 mouse = GetMousePosition();
    slider->wasHovered = slider->isHovered;
    slider->wasPressed = slider->isPressed;
    slider->isHovered = Sol_Check_2d_Collision(
        (Vec2){mouse.x, mouse.y},
        slider->SolRect);

    if (slider->isHovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        slider->isPressed = 1;
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        slider->isPressed = 0;
    if (slider->isPressed)
    {
        float localX = mouse.x - slider->rect.x;
        float newValue = localX / slider->rect.width;
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
    Color handleColor = slider->isPressed ? DARKGRAY : slider->handleC;
    DrawRectangleRec(slider->rect, drawColor);
    if (slider->isHovered)
        DrawRectangleLinesEx(slider->rect, 2.0f, BLUE);
    DrawRectangleRec((Rectangle){
                         slider->rect.x,
                         slider->rect.y,
                         slider->rect.width * slider->value,
                         slider->rect.height,
                     },
                     slider->fillC);

    DrawRectangleRec((Rectangle){
                         .x = slider->rect.x + (slider->rect.width * slider->value) - slider->handleWidth / 2.0f,
                         slider->rect.y,
                         slider->handleWidth,
                         slider->rect.height,
                     },
                     handleColor);
    // Center text logic
    int fontSize = 20;
    int textWidth = MeasureText(slider->text, fontSize);
    DrawText(slider->text,
             slider->rect.x + (slider->rect.width / 2) - (textWidth / 2),
             slider->rect.y + (slider->rect.height / 2) - (fontSize / 2),
             fontSize, WHITE);
}
