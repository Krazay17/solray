#include "core/App.h"
#include "World.h"
#include "modules/PhysxSystem.h"
#include <stdlib.h>
#include "core/Loader.h"
#include "net/Net.h"

// Forward declare the menu factory so we can return to it
extern World *CreateMenuWorld();

#define MAX_ENTITIES 100

typedef struct
{
    NetState network;
    Sound sound;
    Mesh knotMesh;
    Model knotModel;
    Model *playerModel;
    int entities;

    Body bodies[MAX_ENTITIES];

} State;

static void CreatePlayer(State *s)
{
    s->bodies[s->entities] = (Body){
        .position = (Vector3){0, 5, 0},
        .velocity = (Vector3){0, 5, 0},
        .accelG = 2.0f,
        .accelA = 1.0f,
        .frictionA = 0.0f,
        .frictionG = 0.5f,
        .mass = 100.0f,
    };
    s->entities++;
}

static void Init(World *self)
{
    State *s = (State *)self->state;

    s->entities = 0;
    s->playerModel = &GetRM()->draw.cylinderModel;
    s->knotMesh = GenMeshKnot(1, 1, 12, 24);
    s->knotModel = LoadModelFromMesh(s->knotMesh);

    DisableCursor();
    PlaySound(GetRM()->audio.woong1);

    CreatePlayer(s);
}

static void Step(World *self, float dt)
{
    State *s = (State *)self->state;
    NetPoll(&s->network);

    Update_Physx(s->bodies, s->entities, dt);
}

static void Tick(World *self, float dt)
{
    State *s = (State *)self->state;
    if (IsKeyPressed(KEY_BACKSPACE))
        SwitchWorld(CreateMenuWorld());
    if (IsKeyPressed(KEY_ESCAPE) && IsCursorHidden())
        EnableCursor();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (IsCursorHidden())
            PlaySound(GetRM()->audio.pistolFire);
        else
            DisableCursor();
    }
}

static void Draw(World *self)
{
    State *s = (State *)self->state;

    BeginMode3D(globalCamera);
    for (int i = 0; i < s->entities; i++)
    {
        Body body = s->bodies[i];
        DrawModel(*s->playerModel, body.position, 1.0f, RED);
        DrawModelWires(*s->playerModel, body.position, 1, BLACK);
    }
    DrawModel(s->knotModel, (Vector3){0, 5, 0}, 1.0f, BLUE);
    DrawModelWires(s->knotModel, (Vector3){0, 5, 0}, 1.0f, BLACK);

    DrawGrid(20, 1.0f);
    EndMode3D();

    DrawText("LEVEL 1", 10, 10, 20, DARKGRAY);

    UpdateCamera(&globalCamera, CAMERA_THIRD_PERSON);
}

static void Exit(World *self)
{
    State *s = (State *)self->state;
    UnloadModel(s->knotModel);
}

typedef struct
{
    World world;
    State state;
} Container;
World *CreateLevel1World()
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