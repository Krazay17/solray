#include "Button.h"

bool UpdateButton(Button *btn) {
    Vector2 mouse = GetMousePosition();
    btn->isHovered = CheckCollisionPointRec(mouse, btn->rect);
    
    if (btn->isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return true; // The button was clicked
    }
    return false;
}

void DrawButton(Button *btn, const char *text) {
    Color drawColor = btn->isHovered ? SKYBLUE : btn->baseColor;
    
    DrawRectangleRec(btn->rect, drawColor);
    // Add a simple border if hovered
    if (btn->isHovered) DrawRectangleLinesEx(btn->rect, 2, BLUE);

    // Center text logic
    int fontSize = 20;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, 
             btn->rect.x + (btn->rect.width/2) - (textWidth/2), 
             btn->rect.y + (btn->rect.height/2) - (fontSize/2), 
             fontSize, WHITE);
}
