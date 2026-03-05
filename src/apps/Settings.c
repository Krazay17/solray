#include "core/App.h"
#include "core/World.h"
#include <stdlib.h>
#include <stdio.h>
#include "ui/Button.h"
#include "core/GlobalState.h"
#include "core/LocalConfig.h"
#include "raymath.h"

typedef enum
{
    BTN_FULLSCREEN,
    BTN_BORDERLESS,
    BTN_AONTOP,
    BTN_COUNT,
} SettingsButtonId;

typedef enum
{
    WINDOWSTATE_WINDOWED,
    WINDOWSTATE_FULLSCREEN,
    WINDOWSTATE_BORDERLESS,
} WindowState;

typedef enum
{
    SLDR_VOLUME,
    SLRD_OPACITY,
    SLDR_COUNT,
} SettingsSliderId;

typedef struct
{
    Button buttons[BTN_COUNT];
    Slider sliders[SLDR_COUNT];
    WindowState windowState;
    bool onTop;
    bool borderless;
    bool fullscreen;
} State;

static void Init(World *self)
{
    State *s = (State *)self->state;

    float startY = 150.0f;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        s->buttons[i] = (Button){
            .rect = {400.0f, startY + i * 60.0f, 150.0f, 30.0f},
            .baseColor = GRAY,
            .isPressed = false,
            .isHovered = false,
        };
    }

    for (int i = 0; i < SLDR_COUNT; i++)
    {
        s->sliders[i] = (Slider){
            .bg = {100.0f, startY + i * 60, 150.0f, 30.0f},
            .bgC = GRAY,
            .fillC = GREEN,
            .isPressed = false,
            .isHovered = false,
            .handleC = RED,
            .handleWidth = 15.0f,
        };
    }
    s->buttons[BTN_AONTOP].text = "OnTop";
    s->buttons[BTN_BORDERLESS].text = "Borderless";
    s->buttons[BTN_FULLSCREEN].text = "Fullscreen";
    s->sliders[SLDR_VOLUME].text = "Volume";
    s->sliders[SLDR_VOLUME].value = LocalConfig.volume;
    s->sliders[SLRD_OPACITY].text = "Opacity";
    s->sliders[SLRD_OPACITY].value = LocalConfig.opacity;
}

static void Open(World *self)
{
}

static void Step(World *self, float delta)
{
    State *s = (State *)self->state;
}

static void Tick(World *self, float dt)
{
    State *s = (State *)self->state;

    if (IsKeyPressed(KEY_BACKSPACE))
        SwitchWorld(GetMenuWorld());

    float startY = 150.0f;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        if (UpdateButton(&s->buttons[i], dt))
        {
            if (i == BTN_FULLSCREEN)
            {
                if (s->windowState != WINDOWSTATE_FULLSCREEN)
                    s->windowState = WINDOWSTATE_FULLSCREEN;
                else
                    s->windowState = WINDOWSTATE_WINDOWED;
            }
            if (i == BTN_BORDERLESS)
            {
                if (s->windowState != WINDOWSTATE_BORDERLESS)
                    s->windowState = WINDOWSTATE_BORDERLESS;
                else
                    s->windowState = WINDOWSTATE_WINDOWED;
            }
            if (i == BTN_AONTOP)
            {
                s->onTop = !s->onTop;
                if (s->onTop)
                {
                    SetWindowState(FLAG_WINDOW_TOPMOST);
                    s->buttons[i].baseColor = GREEN;
                }
                else
                {
                    ClearWindowState(FLAG_WINDOW_TOPMOST);
                    s->buttons[i].baseColor = GRAY;
                }
            }
            switch (s->windowState)
            {
            case WINDOWSTATE_WINDOWED:
                ClearWindowState(FLAG_FULLSCREEN_MODE | FLAG_BORDERLESS_WINDOWED_MODE);
                s->buttons[BTN_FULLSCREEN].baseColor = GRAY;
                s->buttons[BTN_BORDERLESS].baseColor = GRAY;
                break;
            case WINDOWSTATE_FULLSCREEN:
                ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
                SetWindowState(FLAG_FULLSCREEN_MODE);
                s->buttons[BTN_FULLSCREEN].baseColor = GREEN;
                s->buttons[BTN_BORDERLESS].baseColor = GRAY;
                break;
            case WINDOWSTATE_BORDERLESS:
                ClearWindowState(FLAG_FULLSCREEN_MODE);
                SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
                s->buttons[BTN_BORDERLESS].baseColor = GREEN;
                s->buttons[BTN_FULLSCREEN].baseColor = GRAY;
                break;
            default:
                break;
            }
        }
    }

    for (int i = 0; i < SLDR_COUNT; i++)
    {
        if (UpdateSlider(&s->sliders[i]))
        {
            float value = s->sliders[i].value;

            if (i == SLDR_VOLUME)
            {
                SetMasterVolume(value);
                LocalConfig.volume = value;
            }
            if (i == SLRD_OPACITY)
            {
                float limit = Lerp(0.2f, 1.0f, value);
                SetWindowOpacity(limit);
                LocalConfig.opacity = limit;
            }
        }
        if (s->sliders[i].wasPressed && !s->sliders[i].isPressed)
        {
            Save_Config(&LocalConfig);
        }
    }
}

static void Draw(World *self)
{
    State *s = (State *)self->state;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        DrawButton(&s->buttons[i]);
    }
    for (int i = 0; i < SLDR_COUNT; i++)
    {
        DrawSlider(&s->sliders[i]);
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

State settingsState = {0};
World settingsWorld = {
    .Init = Init,
    .Open = Open,
    .Step = Step,
    .Tick = Tick,
    .Draw = Draw,
    .Exit = Exit,
    .Kill = Kill,
    .staticFlag = 1,
    .state = &settingsState,
};

World *GetSettingsWorld()
{
    return &settingsWorld;
}

// typedef struct
// {
//     World world;
//     State state;
// } Container;
// World *CreateSettingsWorld()
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