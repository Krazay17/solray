#include "core/App.h"
#include "core/World.h"
#include "core/GlobalState.h"
#include "ui/Button.h"

typedef enum
{
    BTN_GAME,
    BTN_FULLSCREEN,
    BTN_QUIT,
    BTN_COUNT,
} BtnId;

typedef struct State
{
    Button buttons[BTN_COUNT];
} State;

static void Init(World *self) {}
static void Open(World *self) {}
static void Tick(World *self, float dt) {}
static void Draw(World *self)
{
    DrawRectangleGradientV(0, 0, engineState.width, engineState.height, BLACK, BLANK);
}
static void Exit() {}

static State state = {0};
static World pMenuWorld = {
    .Init = Init,
    .Open = Open,
    .Tick = Tick,
    .Draw = Draw,
    .Exit = Exit,
    .state = &state,
    .staticFlag = 1,
};

World *GetPMenuWorld()
{
    return &pMenuWorld;
}