#include "core/App.h"
#include <stdlib.h>
#include "hexed/test_sound.h"

// Forward declare the menu factory so we can return to it
extern Scene *CreateMenuScene();

typedef struct
{
    Sound sound;
} Level1Data;

static void Level1Init(Scene *self)
{
    self->data = malloc(sizeof(Level1Data));
    Level1Data *d = (Level1Data*)self->data;

    Wave wave = LoadWaveFromMemory(".mp3", Test1_mp3, Test1_mp3_len);
    Sound testSound = LoadSoundFromWave(wave);
    d->sound = testSound;

    PlaySound(d->sound);

    DisableCursor();
}

static void Level1Update(Scene *self, float delta)
{
    UpdateCamera(&globalCamera, CAMERA_THIRD_PERSON);
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        SwitchScene(CreateMenuScene());
    }

    // globalCamera.target
}

static void Level1Draw(Scene *self)
{
    BeginMode3D(globalCamera);
    DrawCube((Vector3){0, 0, 0}, 2.0f, 2.0f, 2.0f, RED);
    DrawCubeWires((Vector3){0, 0, 0}, 2.0f, 2.0f, 2.0f, MAROON);
    DrawGrid(10, 1.0f);
    EndMode3D();

    DrawText("LEVEL 1", 10, 10, 20, DARKGRAY);
}

static void Level1Unload(Scene *self)
{
    free(self->data);
}

// The only public function for this file
Scene *CreateLevel1Scene()
{
    Scene *s = malloc(sizeof(Scene));
    s->Init = Level1Init;
    s->Update = Level1Update;
    s->Draw = Level1Draw;
    s->Unload = Level1Unload;
    return s;
}