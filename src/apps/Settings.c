#include "core/App.h"
#include <stdlib.h>
#include <stdio.h>
#include "ui/Button.h"
#include "core/GlobalState.h"

extern Scene *CreateMenuScene();

typedef enum
{
    BTN_VOLUME,
    BTN_COUNT,
} SettingsButtonId;

typedef enum
{
    SLDR_VOLUME,
    SLDR_COUNT,
} SettingsSliderId;

typedef struct
{
    Button buttons[BTN_COUNT];
    Slider sliders[SLDR_COUNT];
} SettingsState;

static void SettingsInit(Scene *self)
{
    self->state = malloc(sizeof(SettingsState));
    SettingsState *s = (SettingsState *)self->state;

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
}

static void SettingsUpdate(Scene *self, float delta)
{
    SettingsState *s = (SettingsState *)self->state;

    if (IsKeyPressed(KEY_BACKSPACE))
        SwitchScene(CreateMenuScene());

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
            {
                SetMasterVolume(s->sliders[i].value);
            }
        }
    }
}

static void SettingsDraw(Scene *self)
{
    SettingsState *s = (SettingsState *)self->state;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        DrawButton(&s->buttons[i]);
    }
    for (int i = 0; i < SLDR_COUNT; i++)
    {
        DrawSlider(&s->sliders[i]);
    }
}

static void SettingsUnload(Scene *self)
{
    free(self->state);
}

Scene *CreateSettingsScene()
{
    Scene *s = malloc(sizeof(Scene));
    s->Init = SettingsInit;
    s->Update = SettingsUpdate;
    s->Draw = SettingsDraw;
    s->Unload = SettingsUnload;
    return s;
}