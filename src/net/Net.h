#pragma once
#include <stdbool.h>
#include "../server/Server.h"

typedef struct NetState
{
    PlayerPacket players[MAX_CLIENTS];
    int localId;    // The ID the server gave US
    bool connected; // Are we actually talking to a server?
    int length;
} NetState;

// Simple Global Interface
bool NetInit(void);                        // Init ENet
bool NetConnect(const char *ip, int port); // Connect to a server
void NetService(void);                     // Pump the network (Call in main loop)
void NetDeinit(void);                      // Shutdown

// Data Exchange
const NetState *NetPoll(void); // Get latest world state
void NetSendLocalPos(float x, float y, float z);