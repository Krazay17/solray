#define ENET_IMPLEMENTATION
#include "enet.h"
#include <stdio.h>
#include <stdbool.h>

// Copy your structs here (or share a header)
typedef struct
{
    int id;
    float x, y, z;
    bool active;
} RemotePlayer;

int main()
{
    if (enet_initialize() != 0)
    {
        printf("Could not initialize ENet.\n");
        return 1;
    }

    ENetAddress address = {0};
    address.host = ENET_HOST_ANY;
    address.port = 7777; // Match this in your client!

    // Create a server host
    // 32 slots, 2 channels, no bandwidth limits
    ENetHost *server = enet_host_create(&address, 32, 2, 0, 0);

    if (server == NULL)
    {
        printf("Could not create server host. \n");
        return 1;
    }

    printf("Server started on port 7777...\n");

    ENetEvent event;
    while (true)
    {
        /* Wait up to 1000 milliseconds for an event. (WARNING: blocking) */
        while (enet_host_service(server, &event, 1000) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
                /* Store any relevant client information here. */
                event.peer->data = "Client information";
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                
                /* Clean up the packet now that we're done using it. */
                RemotePlayer *p = (RemotePlayer *)event.packet->data;
                printf("X: %f Y: %f Z: %f\n", p->x, p->y, p->z);
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%s disconnected.\n", event.peer->data);
                /* Reset the peer's client information. */
                event.peer->data = NULL;
                break;

            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                printf("%s disconnected due to timeout.\n", event.peer->data);
                /* Reset the peer's client information. */
                event.peer->data = NULL;
                break;

            case ENET_EVENT_TYPE_NONE:
                break;
            }
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();
    return 0;
}