#define WIN32_LEAN_AND_MEAN
#define ENET_IMPLEMENTATION
#include <enet.h>
#include "Net.h"
#include <stdio.h>

#define DEFAULT_IP "answer-cuba.gl.at.ply.gg"
#define DEFAULT_PORT 35101
#define RECONNECT_INTERVAL 500
#define MAX_RECONNECT_INTERVAL 30000

// typedef enum
// {
//     NET_STATUS_DISCONNECTED,
//     NET_STATUS_CONNECTING,
//     NET_STATUS_CONNECTED,
// } NetConnectStatus;

typedef struct
{
    const char *ip;
    int port;
    ENetAddress address;
    uint8_t initialized;
} NetConfig;

static uint32_t last_reconnect_attempt = 0;
static uint32_t reconnect_accum = 0;
static ENetHost *internal_client = {0};
static ENetPeer *server_peer = {0};
static NetState internal_state = {0};
static NetConfig netConfig = {0};

bool NetInit(void)
{
    if (enet_initialize() != 0)
        return false;

    netConfig.ip = DEFAULT_IP;
    netConfig.port = DEFAULT_PORT;
    enet_address_set_host(&netConfig.address, netConfig.ip);
    netConfig.address.port = netConfig.port;
    netConfig.initialized = 1;

    return true;
}

bool NetConnect(const char *ip, int port)
{
    if (ip)
    {
        netConfig.ip = ip;
        netConfig.port = port;
        enet_address_set_host(&netConfig.address, netConfig.ip);
        netConfig.address.port = netConfig.port;
    }
    if (!internal_client)
        internal_client = enet_host_create(NULL, 1, 2, 0, 0);
    if (!internal_client)
        return false;
    if (server_peer)
        enet_peer_reset(server_peer);
    server_peer = enet_host_connect(internal_client, &netConfig.address, 2, 0);

    return server_peer != NULL;
}

void NetService(void)
{
    if (!internal_client)
        return;
    if (internal_state.connected == false)
    {
        uint32_t now = enet_time_get();
        if (now - last_reconnect_attempt > RECONNECT_INTERVAL + reconnect_accum)
        {
            last_reconnect_attempt = now;
            NetConnect(NULL, 0);

            printf("Attempting Reconnect. Interval: %d\n", RECONNECT_INTERVAL + reconnect_accum);

            reconnect_accum += 100;
            if (reconnect_accum > MAX_RECONNECT_INTERVAL)
                reconnect_accum = MAX_RECONNECT_INTERVAL;
        }
    }
    else
        reconnect_accum = 0;

    ENetEvent event = {0};
    while (enet_host_service(internal_client, &event, 0) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            internal_state.connected = true;
            printf("Client ReConnected\n");
            break;
        case ENET_EVENT_TYPE_RECEIVE:
        {
            // Welcome Packet
            if (event.packet->dataLength == sizeof(uint8_t))
            {
                int id = *(uint8_t *)event.packet->data;
                internal_state.localId = id;
                internal_state.connected = true;

                printf("Welcome Packet ID: %d\n", id);
            }
            else if (event.packet->dataLength == sizeof(WorldState))
            {
                WorldState *inc = (WorldState *)event.packet->data;
                memcpy(internal_state.players, inc->players, sizeof(PlayerPacket) * MAX_CLIENTS);
            }
            enet_packet_destroy(event.packet);
            break;
        }
        case ENET_EVENT_TYPE_DISCONNECT:
            internal_state.connected = false;
            internal_state.localId = -1;
            memset(internal_state.players, 0, sizeof(PlayerPacket) * MAX_CLIENTS);
            server_peer = NULL;
            break;
        case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
            internal_state.connected = false;
            internal_state.localId = -1;
            memset(internal_state.players, 0, sizeof(PlayerPacket) * MAX_CLIENTS);
            server_peer = NULL;
            break;
        case ENET_EVENT_TYPE_NONE:
            break;
        }
    }
}

const NetState *NetPoll(void)
{
    return &internal_state;
}

void NetSendLocalPos(float x, float y, float z)
{
    if (!server_peer)
        return;

    PlayerPacket myMsg = {.id = internal_state.localId, .x = x, .y = y, .z = z};
    ENetPacket *packet = enet_packet_create(&myMsg, sizeof(PlayerPacket), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    enet_peer_send(server_peer, 0, packet);
}

void NetDeinit(void)
{
    if (internal_client)
        enet_host_destroy(internal_client);
    enet_deinitialize();
}