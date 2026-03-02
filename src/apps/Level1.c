#include "core/App.h"
#include "World.h"
#include "modules/InputSystem.h"
#include "modules/MoveSystem.h"
#include "modules/PhysxSystem.h"
#include "modules/CamSystem.h"
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
    int localId;

    Input inputs[MAX_ENTITIES];
    Body bodies[MAX_ENTITIES];
    CamControl camControl;
} State;

static int AddEntity(State *s)
{
    int id = s->entities;
    s->bodies[id] = (Body){0};
    s->inputs[id] = (Input){0};

    s->entities++;
    return id;
}
static void CreatePlayer(State *s)
{
    s->localId = AddEntity(s);

    s->bodies[s->localId] = (Body){
        .position = (Vector3){0, 5, 0},
        .velocity = (Vector3){0, 5, 0},
        .accelG = 15.0f,
        .accelA = 5.0f,
        .frictionG = 0.5f,
        .frictionA = 0.0f,
        .speedG = 5.0f,
        .speedA = 1.0f,
        .mass = 100.0f,
    };
    s->camControl = (CamControl){
        .yaw = 0,
        .pitch = 0,
        .sens = 0.2f,
    };
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
    if (IsCursorHidden())
        Update_Cam(&globalCamera, &s->bodies[s->localId], &s->camControl);

    Update_Input(s->inputs, s->entities, s->localId, globalCamera);
    Update_Move(s->inputs, s->bodies, s->entities, dt);
    Update_Physx(s->bodies, s->entities, dt);
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

    DrawGrid(50, 1.0f);
    EndMode3D();

    DrawText("LEVEL 1", 10, 10, 20, DARKGRAY);
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