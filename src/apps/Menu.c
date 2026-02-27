#include "core/App.h"
#include <stdlib.h>

// Forward declare the menu factory so we can return to it
extern Scene *CreateLevel1Scene();

typedef struct
{
    Rectangle btn;
} MenuData;

static void MenuInit(Scene *self)
{
    self->data = malloc(sizeof(MenuData));
    ((MenuData *)self->data)->btn = (Rectangle){300, 200, 200, 50};

    EnableCursor();
}

static void MenuUpdate(Scene *self, float delta)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(GetMousePosition(), ((MenuData *)self->data)->btn))
    {
        SwitchScene(CreateLevel1Scene());
    }
}

static void MenuDraw(Scene *self)
{
    DrawRectangleRec(((MenuData *)self->data)->btn, GRAY);
    DrawText("Start Level 1", 340, 215, 20, WHITE);
}

static void MenuUnload(Scene *self) { free(self->data); }

Scene *CreateMenuScene()
{
    Scene *s = malloc(sizeof(Scene));
    s->Init = MenuInit;
    s->Update = MenuUpdate;
    s->Draw = MenuDraw;
    s->Unload = MenuUnload;
    return s;
}