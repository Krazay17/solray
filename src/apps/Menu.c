#include "core/App.h"
#include "World.h"
#include <stdlib.h>
#include "ui/Button.h"
#include "core/Loader.h"

// Forward declare the menu factory so we can return to it
extern World *CreateLevel1World();
extern World *CreateSettingsWorld();

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
    Sound hoverSound;
    int currentChannel;
} State;

static void Init(World *self)
{
    State *d = (State *)self->state;

    float startY = 150.0f;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        d->buttons[i] = (Button){{300, startY + (i * 60), 200, 50}, GRAY, false, false};
    }

    d->hoverSound = GetRM()->audio.beep1;
    d->buttons[BTN_START].text = "Start Game";
    d->buttons[BTN_SETTINGS].text = "Settings";
    d->buttons[BTN_QUIT].text = "Quit";

    if (IsCursorHidden())
        EnableCursor();
    Sound music = GetRM()->audio.menuMusic;
    SetSoundVolume(music, 0.4f);
    PlaySound(music);
}

static void Step(World *self, float delta)
{
    State *d = (State *)self->state;
}

static void Tick(World *self, float dt)
{
    State *d = (State *)self->state;

    for (int i = 0; i < BTN_COUNT; i++)
    {
        if (d->buttons[i].isHovered && !d->buttons[i].wasHovered)
        {
            PlaySound(d->hoverSound);
        }
        if (UpdateButton(&d->buttons[i]))
        {
            if (i == BTN_START)
            {
                SwitchWorld(CreateLevel1World());
                PlaySound(GetRM()->audio.beep2);
                return;
            }
            if (i == BTN_SETTINGS)
            {
                SwitchWorld(CreateSettingsWorld());
                PlaySound(GetRM()->audio.beep2);
                return;
            }
            if (i == BTN_QUIT)
            {
                AppShouldClose = true;
                return;
            }
        }
    }
}

static void Draw(World *self)
{
    State *d = (State *)self->state;

    for (int i = 0; i < BTN_COUNT; i++)
    {
        DrawButton(&d->buttons[i]);
    }
}

static void Exit(World *self)
{
}

typedef struct
{
    World world;
    State state;
} Container;
World *CreateMenuWorld()
{
    Container *w = malloc(sizeof(Container));

    w->world.Init = Init;
    w->world.Step = Step;
    w->world.Tick = Tick;
    w->world.Draw = Draw;
    w->world.Exit = Exit;

    w->world.state = &w->state;

    return (World *)w;
}