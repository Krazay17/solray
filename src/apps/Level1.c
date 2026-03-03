#include "core/App.h"
#include <stdio.h>
#include "core/World.h"
#include "core/GameWorld.h"
#include <stdlib.h>
#include "core/Loader.h"
#include "net/Net.h"
#include "ui/Debug.h"

// Forward declare the menu factory so we can return to it
extern World *CreateMenuWorld();

static int AddEntity(GameState *s)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (!s->entities.active[i])
        {
            s->entities.active[i] = true;
            s->bodies[i] = (Body){0};
            s->inputs[i] = (Input){0};
            // Keep track of the highest index to optimize loops if you want
            if (i >= s->entities.count)
                s->entities.count = i + 1;
            return i;
        }
    }
    return -1; // Out of memory!
}

int CreatePlayer(GameState *s)
{
    int id = AddEntity(s);

    s->bodies[id] = (Body){
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

    return id;
}

static void Init(World *self)
{
    GameState *s = (GameState *)self->state;
    s->entities.count = 0;
    s->playerModel = &GetRM()->draw.cylinderModel;
    s->knotMesh = GenMeshKnot(1, 1, 12, 24);
    s->knotModel = LoadModelFromMesh(s->knotMesh);
    s->localId = 0;
    s->netLocalId = -1;
    for (int i = 0; i < MAX_CLIENTS; i++)
        s->netToLocal[i] = -1;

    DisableCursor();
    PlaySound(GetRM()->audio.woong1);

    CreatePlayer(s);
}

static void Step(World *self, float dt)
{
    GameState *s = (GameState *)self->state;
    Update_NetSync(s, dt);

    debugs[DEBUG_FRAMERATE].value = GetFPS();
    debugs[DEBUG_LOCALID].value = s->localId;
    debugs[DEBUG_ENTITIES].value = s->entities.count;
    debugs[DEBUG_NETID].value = s->netLocalId;
}

static void Tick(World *self, float dt)
{
    GameState *s = (GameState *)self->state;
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

    Update_Input(s->inputs, &s->entities, s->localId, globalCamera);
    Update_Move(s->inputs, s->bodies, &s->entities, dt);
    Update_Physx(s->bodies, &s->entities, dt);
}

static void Draw(World *self)
{
    GameState *s = (GameState *)self->state;

    BeginMode3D(globalCamera);
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (!s->entities.active[i])
            continue;
        Body body = s->bodies[i];
        DrawModel(*s->playerModel, body.position, 1.0f, RED);
        DrawModelWires(*s->playerModel, body.position, 1, BLACK);
    }
    DrawModel(s->knotModel, (Vector3){0, 5, 0}, 1.0f, BLUE);
    DrawModelWires(s->knotModel, (Vector3){0, 5, 0}, 1.0f, BLACK);

    DrawGrid(50, 1.0f);
    EndMode3D();

    Draw_Debug(debugs, DEBUG_COUNT);
}

static void Exit(World *self)
{
    GameState *s = (GameState *)self->state;
    UnloadModel(s->knotModel);
}

static GameState gameState = {
    .entities = 0,
};
static World GameWorld = {
    .Init = Init,
    .Step = Step,
    .Tick = Tick,
    .Draw = Draw,
    .Exit = Exit,
    .staticFlag = 1,
    .state = &gameState,
};

World *GetGameWorld()
{
    return &GameWorld;
}
