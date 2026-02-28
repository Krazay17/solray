#include "core/App.h"
#include <stdlib.h>
#include "ui/Button.h"

// Forward declare the menu factory so we can return to it
extern Scene *CreateLevel1Scene();

typedef enum
{
    BTN_START,
    BTN_SETTINGS,
    BTN_QUIT,
    BTN_COUNT
} MenuButtonId;

typedef struct
{
    Button buttons[BTN_COUNT];
    const char *labels[BTN_COUNT];
} MenuData;

static void MenuInit(Scene *self)
{
    self->state = malloc(sizeof(MenuData));
    MenuData *d = (MenuData *)self->state;

    float startY = 150.0f;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        d->buttons[i] = (Button){{300, startY + (i * 60), 200, 50}, GRAY, false, false};
    }

    d->labels[BTN_START] = "Start Game";
    d->labels[BTN_SETTINGS] = "Settings";
    d->labels[BTN_QUIT] = "Quit";
    EnableCursor();
}

static void MenuUpdate(Scene *self, float delta)
{
    MenuData *d = (MenuData *)self->state;

    for (int i = 0; i < BTN_COUNT; i++) {
        if (UpdateButton(&d->buttons[i])) {
            // Handle specific clicks
            if (i == BTN_START)    SwitchScene(CreateLevel1Scene());
            if (i == BTN_QUIT)     /* Exit Logic */;
        }
    }
}

static void MenuDraw(Scene *self)
{
    // DrawRectangleRec(((MenuData *)self->state)->btn, GRAY);
    // DrawText("Level 1", 340, 215, 20, WHITE);
}

static void MenuUnload(Scene *self) { free(self->state); }

Scene *CreateMenuScene()
{
    Scene *s = malloc(sizeof(Scene));
    s->Init = MenuInit;
    s->Update = MenuUpdate;
    s->Draw = MenuDraw;
    s->Unload = MenuUnload;
    return s;
}