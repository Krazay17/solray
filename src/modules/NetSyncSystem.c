#include "NetSyncSystem.h"
#include "core/GameWorld.h"
#include "net/Net.h"
#include <stdio.h>

int CreatePlayer(GameState *s);

static void SyncNet(GameState *s, const NetState *netState, float dt)
{
    // 1. Send our data
    NetSendLocalPos(
        s->bodies[0].position.x,
        s->bodies[0].position.y,
        s->bodies[0].position.z);

    // 2. Handle Local ID assignment (First time join)
    if (s->netLocalId == -1 && netState->localId != -1)
    {
        s->netLocalId = netState->localId;
        s->netToLocal[netState->localId] = 0;
        printf("Network ID assigned: %d\n", s->netLocalId);
    }

    // 3. Sync others
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (s->netLocalId == i)
            continue;

        int localId = s->netToLocal[i];
        bool remoteActive = netState->players[i].active;

        // Join
        if (remoteActive && localId == -1)
        {
            s->netToLocal[i] = CreatePlayer(s);
            printf("Network Player %d joined.\n", i);
        }
        // Leave
        else if (!remoteActive && localId != -1)
        {
            s->entities.active[localId] = false;
            s->netToLocal[i] = -1;
            printf("Network Player %d left.\n", i);
        }
        // Move
        if (remoteActive && localId != -1)
        {
            s->bodies[localId].position = (Vector3){
                netState->players[i].x,
                netState->players[i].y,
                netState->players[i].z};
        }
    }
}

static void DesyncNet(GameState *s, const NetState *netState)
{
    // If we were previously connected, purge the ghosts
    if (s->netLocalId != -1)
    {
        printf("Connection lost. Cleaning up network entities...\n");
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            int localId = s->netToLocal[i];
            // If this mapping exists and isn't our local player (slot 0)
            if (localId != -1 && localId != 0)
            {
                s->entities.active[localId] = false;
            }
            s->netToLocal[i] = -1; // Reset mapping
        }
        s->netLocalId = -1; // We no longer have a network identity
    }
}

void Update_NetSync(GameState *s, float dt)
{
    NetService();
    const NetState *netState = NetPoll();
    if (netState->connected)
        SyncNet(s, netState, dt);
    else
        DesyncNet(s, netState);
}
