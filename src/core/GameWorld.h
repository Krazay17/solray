#include "modules/InputSystem.h"
#include "modules/MoveSystem.h"
#include "modules/PhysxSystem.h"
#include "modules/CamSystem.h"
#include "modules/NetSyncSystem.h"
#include "net/Net.h"

#define MAX_ENTITIES 100

typedef struct Entities
{
    bool active[MAX_ENTITIES];
    int count;
} Entities;

typedef struct GameState
{
    Sound sound;
    Mesh knotMesh;
    Model knotModel;
    Model *playerModel;
    CamControl camControl;

    int localId;
    int netLocalId;
    Entities entities;
    Input inputs[MAX_ENTITIES];
    Body bodies[MAX_ENTITIES];
    int netToLocal[MAX_CLIENTS];

} GameState;