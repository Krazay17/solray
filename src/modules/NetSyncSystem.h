#pragma once

typedef struct GameState GameState;
typedef struct NetState NetState;

void Update_NetSync(GameState *s, float dt);
static void SyncNet(GameState *s, const NetState *netState, float dt);
static void DesyncNet(GameState *s, const NetState *netState);