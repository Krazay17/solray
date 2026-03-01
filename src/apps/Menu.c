#include "core/App.h"
#include <stdlib.h>
#include "ui/Button.h"
#include "core/Loader.h"

#define MAX_HOVER_CHANNELS 12

// Forward declare the menu factory so we can return to it
extern Scene *CreateLevel1Scene();
extern Scene *CreateSettingsScene();

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
    Sound hoverSounds[MAX_HOVER_CHANNELS];
    int currentChannel;
} MenuState;

static void MenuInit(Scene *self)
{
    self->state = malloc(sizeof(MenuState));
    MenuState *d = (MenuState *)self->state;

    float startY = 150.0f;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        d->buttons[i] = (Button){{300, startY + (i * 60), 200, 50}, GRAY, false, false};
    }

    for (int i = 0; i < MAX_HOVER_CHANNELS; i++)
    {
        d->hoverSounds[i] = LoadSoundAlias(GetRM()->audio.beep1);
    }
    d->buttons[BTN_START].text = "Start Game";
    d->buttons[BTN_SETTINGS].text = "Settings";
    d->buttons[BTN_QUIT].text = "Quit";

    if (IsCursorHidden())
        EnableCursor();
    Sound music = GetRM()->audio.menuMusic;
    SetSoundVolume(music, 0.4f);
    PlaySound(music);
}

static void MenuUpdate(Scene *self, float delta)
{
    MenuState *d = (MenuState *)self->state;

    for (int i = 0; i < BTN_COUNT; i++)
    {
        if (d->buttons[i].isHovered && !d->buttons[i].wasHovered)
        {
            PlaySound(d->hoverSounds[d->currentChannel]);
            d->currentChannel = (d->currentChannel + 1) % MAX_HOVER_CHANNELS;
        }
        if (UpdateButton(&d->buttons[i]))
        {
            if (i == BTN_START)
            {
                SwitchScene(CreateLevel1Scene());
                PlaySound(GetRM()->audio.beep2);
                return;
            }
            if (i == BTN_SETTINGS)
            {
                SwitchScene(CreateSettingsScene());
                PlaySound(GetRM()->audio.beep2);
                return;
            }
            if (i == BTN_QUIT)
            {
                CloseWindow();
                return;
            }
        }
    }
}

static void MenuDraw(Scene *self)
{
    MenuState *d = (MenuState *)self->state;
    for (int i = 0; i < BTN_COUNT; i++)
    {
        DrawButton(&d->buttons[i]);
    }
}

static void MenuUnload(Scene *self)
{
    MenuState *d = (MenuState *)self->state;
    if (d)
    {
        for (int i = 0; i < MAX_HOVER_CHANNELS; i++)
        {
            UnloadSoundAlias(d->hoverSounds[i]);
        }
        free(d);
    }
}

Scene *CreateMenuScene()
{
    Scene *s = malloc(sizeof(Scene));
    s->Init = MenuInit;
    s->Update = MenuUpdate;
    s->Draw = MenuDraw;
    s->Unload = MenuUnload;
    return s;
}