#include "core/App.h"
#include "core/World.h"
#include <stdlib.h>
#include <stdio.h>
#include "ui/Button.h"
#include "core/GlobalState.h"
#include "core/LocalConfig.h"
#include "raymath.h"
#include "modules/EventSystem.h"

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

State settingsState;

static void ReSize(void *target, void *data)
{
    World *self = (World*)target;
    WindowData *wData = (WindowData*)data;
    float startY = 150.0f;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        settingsState.buttons[i].rect.x = engineState.width / 4.0f;
        settingsState.buttons[i].rect.y = startY + i * 60;
    }
    for (int i = 0; i < SLDR_COUNT; i++)
    {
        settingsState.sliders[i].bg.x = engineState.width / 1.5f;
        settingsState.sliders[i].bg.y = startY + i * 60;
    }
}
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
    WindowData windowData = {
        .width = engineState.width,
        .height = engineState.height,
    };
    ReSize(self, &windowData);
    OnEvent(EVENT_WINDOW_RESIZE, self, ReSize);

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
    if (IsCursorHidden())
        EnableCursor();
}

static bool Poll(World *self, float dt)
{
    State *s = (State *)self->state;
    bool consumed = false;

    // 1. Handle Global Input (Back to Menu)
    if (IsKeyPressed(KEY_ESCAPE))
    {
        OpenWorld(WORLD_GAME);
        return true;
    }

    // 2. Handle Buttons
    for (int i = 0; i < BTN_COUNT; i++)
    {
        // UpdateButton should check if mouse is over/clicking
        if (UpdateButton(&s->buttons[i], dt))
        {
            consumed = true;

            // Handle the specific logic for this button
            if (i == BTN_FULLSCREEN)
            {
                s->windowState = (s->windowState == WINDOWSTATE_FULLSCREEN) ? WINDOWSTATE_WINDOWED : WINDOWSTATE_FULLSCREEN;
            }
            else if (i == BTN_BORDERLESS)
            {
                s->windowState = (s->windowState == WINDOWSTATE_BORDERLESS) ? WINDOWSTATE_WINDOWED : WINDOWSTATE_BORDERLESS;
            }
            else if (i == BTN_AONTOP)
            {
                s->onTop = !s->onTop;
            }
        }
        // Even if not clicked, if it's hovered, we consume input to block layers below
        if (s->buttons[i].isHovered)
            consumed = true;
    }

    // 3. Handle Sliders
    for (int i = 0; i < SLDR_COUNT; i++)
    {
        if (UpdateSlider(&s->sliders[i]))
        {
            consumed = true;
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

        // Save config when the user lets go of the slider
        if (s->sliders[i].wasPressed && !s->sliders[i].isPressed)
        {
            Save_Config(&LocalConfig);
        }

        if (s->sliders[i].isHovered || s->sliders[i].isPressed)
            consumed = true;
    }

    return consumed;
}

static void Step(World *self, float delta)
{
    State *s = (State *)self->state;
}

static void Tick(World *self, float dt)
{
    State *s = (State *)self->state;

    // Sync Window States
    if (s->onTop)
    {
        SetWindowState(FLAG_WINDOW_TOPMOST);
        s->buttons[BTN_AONTOP].baseColor = GREEN;
    }
    else
    {
        ClearWindowState(FLAG_WINDOW_TOPMOST);
        s->buttons[BTN_AONTOP].baseColor = GRAY;
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
    }
}

static void Draw(World *self)
{
    State *s = (State *)self->state;

    DrawRectangleGradientV(0, 0, engineState.width, engineState.height, BLACK, BLANK);

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
    .Poll = Poll,
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