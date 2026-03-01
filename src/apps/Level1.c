#include "core/App.h"
#include <stdlib.h>
#include "core/Loader.h"
#include "net/Net.h"

// Forward declare the menu factory so we can return to it
extern Scene *CreateMenuScene();

typedef struct
{
    NetState network;
    Sound sound;
    Mesh knotMesh;
    Material defMat;
    Model knotModel;
} Level1State;

Matrix matDefault = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

static void Level1Init(Scene *self)
{
    self->state = malloc(sizeof(Level1State));
    Level1State *s = (Level1State *)self->state;

    DisableCursor();

    PlaySound(GetRM()->audio.woong1);

    s->knotMesh = GenMeshKnot(5.0f, 2.0f, 24, 24);
    s->defMat = LoadMaterialDefault();
    s->knotModel = LoadModelFromMesh(s->knotMesh);
}

static void Level1Update(Scene *self, float delta)
{
    Level1State *s = (Level1State *)self->state;
    NetPoll(&s->network);
    UpdateCamera(&globalCamera, CAMERA_THIRD_PERSON);
    if (IsKeyPressed(KEY_BACKSPACE))
        SwitchScene(CreateMenuScene());

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        PlaySound(GetRM()->audio.pistolFire);
}

static void Level1Draw(Scene *self)
{
    Level1State *s = (Level1State *)self->state;
    BeginMode3D(globalCamera);
    DrawCube((Vector3){0, 0, 0}, 2.0f, 2.0f, 2.0f, RED);
    DrawCubeWires((Vector3){0, 0, 0}, 2.0f, 2.0f, 2.0f, MAROON);
    DrawGrid(10, 1.0f);
    DrawModel(s->knotModel, (Vector3){0,5,0}, 1.0f, BLUE);
    DrawModelWires(s->knotModel, (Vector3){0,5,0}, 1.0f, BLACK);
    EndMode3D();

    DrawText("LEVEL 1", 10, 10, 20, DARKGRAY);
}

static void Level1Unload(Scene *self)
{
    Level1State *s = (Level1State *)self->state;
    UnloadMaterial(s->defMat);
    UnloadModel(s->knotModel);
    free(self->state);
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