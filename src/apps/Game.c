#include "core/App.h"
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "rlgl.h"
#include "models/rlights.h"
#include "core/World.h"
#include "core/GameWorld.h"
#include "core/Loader.h"
#include "net/Net.h"
#include "ui/Debug.h"

#include "modules/InputSystem.h"
#include "modules/MoveSystem.h"
#include "modules/PhysxSystem.h"
#include "modules/CamSystem.h"
#include "modules/AnimSystem.h"
#include "modules/NetSyncSystem.h"
#include "modules/ViewSystem.h"

Camera3D globalCamera = {
    .projection = CAMERA_PERSPECTIVE,
    .fovy = 45,
    .up = (Vector3){0, 1, 0},
    .position = (Vector3){10, 10, 10},
};

static int AddEntity(GameState *s)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (!s->entities.active[i])
        {
            s->entities.active[i] = true;
            s->bodies[i] = (Body){0};
            s->inputs[i] = (Sol_Input){0};
            // Keep track of the highest index to optimize loops
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

    s->views[id] = (Sol_View){
        .model = *s->wizardModel,
        .yawOffset = 90.0f,
    };

    return id;
}

static void Init(World *self)
{
    GameState *s = (GameState *)self->state;
    s->entities.count = 0;
    s->wizardModel = &GetRM()->models.wizardModel;
    s->playerModel = &GetRM()->models.cylinderModel;
    s->knotMesh = GenMeshKnot(1, 1, 12, 24);
    s->knotModel = LoadModelFromMesh(s->knotMesh);
    s->localId = 0;
    s->netLocalId = -1;
    for (int i = 0; i < MAX_CLIENTS; i++)
        s->netToLocal[i] = -1;

    s->camControl = (CamControl){
        .yaw = 0,
        .pitch = 0,
        .sens = 0.2f,
    };

    CreatePlayer(s);
}

static void Open(World *self)
{
    engineState.worlds[WORLD_MENU]->active = false;
    DisableCursor();
    PlaySound(GetRM()->audio.woong1);
}

static bool Poll(World *self, float dt)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        ChangeMenu(GetMenuWorld());
        OpenWorld(WORLD_MENU);
        return true;
    }
    GameState *s = (GameState *)self->state;
    Input_Update(s->inputs, &s->entities, s->localId, &globalCamera, &s->camControl);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (IsCursorHidden())
            PlaySound(GetRM()->audio.pistolFire);
        else
            DisableCursor();
        return true;
    }
    return false;
}

static void Step(World *self, float dt)
{
    GameState *s = (GameState *)self->state;
    Update_NetSync(s, dt);
    Debug_Clear();
    Debug_Log("Framerate", GetFPS());
    Debug_Log("Entities", s->entities.count);
}

static void Tick(World *self, float dt)
{
    GameState *s = (GameState *)self->state;

    Cam_Update(&globalCamera, &s->bodies[s->localId], &s->camControl, IsCursorHidden());
    Move_Update(s->inputs, s->bodies, &s->entities, dt);
    Update_Physx(s->bodies, &s->entities, dt);
    Anim_Update(s->anims, &s->entities, dt);

    float cameraPos[3] = {globalCamera.position.x, globalCamera.position.y, globalCamera.position.z};
    int viewPosLoc = GetShaderLocation(GetRM()->shaders.light, "viewPos");
    SetShaderValue(GetRM()->shaders.light, viewPosLoc, cameraPos, SHADER_UNIFORM_VEC3);
}

static void Draw(World *self)
{
    GameState *s = (GameState *)self->state;

    BeginMode3D(globalCamera);
    // for (int i = 0; i < MAX_ENTITIES; i++)
    // {
    //     if (!s->entities.active[i])
    //         continue;
    //     Body body = s->bodies[i];
    //     DrawModel(*s->playerModel, body.position, 1.0f, RED);
    //     DrawModelWires(*s->playerModel, body.position, 1, BLACK);
    // }
    // DrawModel(s->knotModel, (Vector3){0, 5, 0}, 1.0f, BLUE);
    // DrawModelWires(s->knotModel, (Vector3){0, 5, 0}, 1.0f, BLACK);

    rlDisableBackfaceCulling();

    // for (int i = 0; i < 1000; i++)
    // {
    //     DrawModel(*s->wizardModel, (Vector3){i + i, 2, 0}, 1.0f, WHITE);
    //     // DrawModelWires(*s->wizardModel, (Vector3){i + i, 2, 0}, 1.0f, BLACK);
    // }

    View_Update(s->views, s->bodies, &s->entities);

    rlEnableBackfaceCulling();

    DrawGrid(50, 1.0f);
    EndMode3D();

    Debug_Draw();
}

static void Exit(World *self)
{
    GameState *s = (GameState *)self->state;
}

static void Kill(World *self)
{
    GameState *s = (GameState *)self->state;
    UnloadModel(s->knotModel);
}

static GameState gameState = {0};
static World GameWorld = {
    .Init = Init,
    .Open = Open,
    .Poll = Poll,
    .Step = Step,
    .Tick = Tick,
    .Draw = Draw,
    .Exit = Exit,
    .Kill = Kill,
    .staticFlag = 1,
    .state = &gameState,
};

World *GetGameWorld()
{
    return &GameWorld;
}
