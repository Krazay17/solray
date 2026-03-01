#pragma once
#include <stdbool.h>

typedef struct {
    int id;
    float x, y, z;
    bool active;
} RemotePlayer;

#define MAX_PLAYERS 50

typedef struct {
    RemotePlayer players[MAX_PLAYERS];
    int localId;      // The ID the server gave US
    bool connected;   // Are we actually talking to a server?
} NetState;

// Simple Global Interface
bool NetInit(void);                      // Init ENet
bool NetConnect(const char* ip, int port); // Connect to a server
void NetService(void);                   // Pump the network (Call in main loop)
void NetDeinit(void);                    // Shutdown

// Data Exchange
void NetPoll(NetState *outState);        // Get latest world state
void NetSendLocalPos(float x, float y, float z);