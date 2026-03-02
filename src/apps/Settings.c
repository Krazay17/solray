#include "core/App.h"
#include "World.h"
#include <stdlib.h>
#include <stdio.h>
#include "ui/Button.h"
#include "core/GlobalState.h"
#include "raymath.h"

extern World *CreateMenuWorld();

typedef enum
{
    BTN_VOLUME,
    BTN_COUNT,
} SettingsButtonId;

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
} State;

static void Init(World *self)
{
    State *s = (State *)self->state;

    float startY = 150.0f;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        s->buttons[i] = (Button){
            .rect = {400.0f, startY + i * 60.0f, 100.0f, 50.0f},
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

    s->sliders[SLDR_VOLUME].text = "Volume";
    s->sliders[SLDR_VOLUME].value = GetMasterVolume();
    s->sliders[SLRD_OPACITY].text = "Opacity";
    s->sliders[SLRD_OPACITY].value = WindowOpacity;
}

static void Step(World *self, float delta)
{
    State *s = (State *)self->state;
}

static void Tick(World *self, float dt)
{
    State *s = (State *)self->state;

    if (IsKeyPressed(KEY_BACKSPACE))
        SwitchWorld(CreateMenuWorld());

    float startY = 150.0f;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        if (UpdateButton(&s->buttons[i]))
        {
            if (i == BTN_VOLUME)
                printf("Click Volume!");
        }
    }

    for (int i = 0; i < SLDR_COUNT; i++)
    {
        if (UpdateSlider(&s->sliders[i]))
        {
            if (i == SLDR_VOLUME)
                SetMasterVolume(s->sliders[i].value);
            if (i == SLRD_OPACITY)
            {
                float limit = Lerp(0.2f, 1.0f, s->sliders[i].value);
                SetWindowOpacity(limit);
                WindowOpacity = limit;
            }
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
}

typedef struct
{
    World world;
    State state;
} Container;
World *CreateSettingsWorld()
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