#include "core/App.h"
#include "core/World.h"
#include <stdlib.h>
#include "ui/Button.h"
#include "core/Loader.h"

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

static void ReSize(World *self, int width, int height)
{
    State *s = (State *)self->state;
    float startY = height / 3.0f;
    float centerX = width / 2.0f;

    for (int i = 0; i < BTN_COUNT; i++)
    {
        // Re-center the button based on new width
        s->buttons[i].rect.x = centerX - (s->buttons[i].rect.width / 2.0f);
        s->buttons[i].rect.y = startY + (i * 60.0f);
    }
}

static void Init(World *self)
{
    State *s = (State *)self->state;

    for (int i = 0; i < BTN_COUNT; i++)
    {
        s->buttons[i] = (Button){{0, 0, 250, 50}, GRAY, false, false};
    }
    ReSize(self, engineState.width, engineState.height);

    s->hoverSound = GetRM()->audio.beep1;
    s->buttons[BTN_START].text = "Start Game";
    s->buttons[BTN_SETTINGS].text = "Settings";
    s->buttons[BTN_QUIT].text = "Quit";

    Sound music = GetRM()->audio.menuMusic;
    PlaySound(music);
    SetSoundVolume(music, 0.4f);
}

static void Open(World *self)
{
    if (IsCursorHidden())
        EnableCursor();
}

static bool Poll(World *self, float dt)
{
    State *s = (State *)self->state;
    bool consumed = false;
    if (IsKeyPressed(KEY_ESCAPE))
    {
        OpenWorld(WORLD_GAME);
        return true;
    }

    for (int i = 0; i < BTN_COUNT; i++)
    {
        if (s->buttons[i].isHovered && !s->buttons[i].wasHovered)
        {
            PlaySound(s->hoverSound);
        }
        if (UpdateButton(&s->buttons[i], dt))
        {
            consumed = true;
            if (i == BTN_START)
            {
                OpenWorld(WORLD_GAME);
            }
            else if (i == BTN_SETTINGS)
            {
                ChangeMenu(GetSettingsWorld());
            }
            else if (i == BTN_QUIT)
            {
                AppShouldClose = true;
            }
        }
    }
    return consumed;
}

static void Step(World *self, float delta)
{
    State *s = (State *)self->state;
}

static void Tick(World *self, float dt)
{
}

static void Draw(World *self)
{
    State *s = (State *)self->state;
    DrawRectangleGradientV(0, 0, engineState.width, engineState.height, BLACK, BLANK);

    for (int i = 0; i < BTN_COUNT; i++)
    {
        DrawButton(&s->buttons[i]);
    }
}

static void Exit(World *self)
{
    State *s = (State *)self->state;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        Button_Reset(&s->buttons[i]);
    }
}

static void Kill(World *self)
{
}

static State menuState = {0};
static World menuWorld = {
    .Init = Init,
    .Open = Open,
    .Poll = Poll,
    .Step = Step,
    .Tick = Tick,
    .Draw = Draw,
    .Exit = Exit,
    .Kill = Kill,
    .ReSize = ReSize,
    .staticFlag = 1,
    .state = &menuState,
    .active = 1,
};

World *GetMenuWorld()
{
    return &menuWorld;
}

// typedef struct
// {
//     World world;
//     State state;
// } Container;
// World *CreateMenuWorld()
// {
//     Container *w = malloc(sizeof(Container));

//     w->world.Init = Init;
//     w->world.Open = Open;
//     w->world.Step = Step;
//     w->world.Tick = Tick;
//     w->world.Draw = Draw;
//     w->world.Exit = Exit;

//     w->world.state = &w->state;

//     Init(&w->world);

//     return (World *)w;
// }