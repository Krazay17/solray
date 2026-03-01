#define WIN32_LEAN_AND_MEAN
#define ENET_IMPLEMENTATION
#include <enet.h>
#include "Net.h"
#include <stdio.h>

static ENetHost *internal_client = {0};
static ENetPeer *server_peer = {0};
static NetState internal_state = {0};

bool NetInit(void)
{
    if (enet_initialize() != 0)
        return false;
    return true;
}

bool NetConnect(const char *ip, int port)
{
    // 1. Create client host
    internal_client = enet_host_create(NULL, 1, 2, 0, 0);
    if (!internal_client)
        return false;

    // 2. Set address
    ENetAddress address = {0};
    enet_address_set_host(&address, ip);
    address.port = port;

    // 3. Connect
    server_peer = enet_host_connect(internal_client, &address, 2, 0);
    ENetEvent event = {0};
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(internal_client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        puts("Connection to some.server.net:1234 succeeded.");
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(server_peer);
        puts("Connection to some.server.net:1234 failed.");
    }
    return server_peer != NULL;
}

void NetService(void)
{
    if (!internal_client)
        return;

    ENetEvent event = {0};
    // Timeout 0 = non-blocking poll
    while (enet_host_service(internal_client, &event, 0) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            printf("Client Connected");
            internal_state.connected = true;
            break;
        case ENET_EVENT_TYPE_RECEIVE:
        {
            RemotePlayer *inc = (RemotePlayer *)event.packet->data;
            if (inc->id >= 0 && inc->id < MAX_PLAYERS)
            {
                internal_state.players[inc->id] = *inc;
                internal_state.players[inc->id].active = true;
            }
            enet_packet_destroy(event.packet);
            break;
        }
        case ENET_EVENT_TYPE_DISCONNECT:
            internal_state.connected = false;
            server_peer = NULL;
            break;
        }
    }
}

void NetPoll(NetState *outState)
{
    *outState = internal_state;
}

void NetSendLocalPos(float x, float y, float z)
{
    if (!server_peer)
        return;

    RemotePlayer myMsg = {.id = internal_state.localId, .x = x, .y = y, .z = z};
    ENetPacket *packet = enet_packet_create(&myMsg, sizeof(RemotePlayer), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    enet_peer_send(server_peer, 0, packet);
}

void NetDeinit(void)
{
    if (internal_client)
        enet_host_destroy(internal_client);
    enet_deinitialize();
}