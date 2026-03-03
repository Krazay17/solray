#pragma once

#define MAX_CLIENTS 100


// #pragma pack(push, 1) // Tells the compiler: "No padding, keep it tight"
// typedef struct
// {
//     uint32_t playerID;
//     float x, y, z;
//     uint8_t inputState;
// } PlayerPacket;
// #pragma pack(pop) // Restore normal alignment for the rest of your code

typedef struct
{
    int id;
    float x, y, z;
    bool active;
} RemotePlayer;

typedef struct
{
    RemotePlayer players[MAX_CLIENTS];
} WorldState;