#include "EventSystem.h"
#include <stdio.h>

#define MAX_LISTENERS 10

typedef struct
{
    EventCallback callback;
    void *target;
} Listener;

typedef struct
{
    Listener listeners[MAX_EVENTS][MAX_LISTENERS];
    int listener_counts[MAX_EVENTS];
} EventSystem;

// Our single global instance
static EventSystem G_EventSystem = {0};

void OnEvent(EventType type, void *target, EventCallback callback)
{
    int count = G_EventSystem.listener_counts[type];
    if (count < MAX_LISTENERS)
    {
        G_EventSystem.listeners[type][count].callback = callback;
        G_EventSystem.listeners[type][count].target = target;
        G_EventSystem.listener_counts[type]++;
    }
}

void EmitEvent(EventType type, void *data)
{
    int count = G_EventSystem.listener_counts[type];
    for (int i = 0; i < count; i++)
    {
        Listener l = G_EventSystem.listeners[type][i];
        l.callback(l.target, data);
    }
}

void OffAll(void *target)
{
    for (int type = 0; type < EVENT_COUNT; type++)
    {
        int count = G_EventSystem.listener_counts[type];
        for (int i = 0; i < count; i++)
        {
            if (G_EventSystem.listeners[type][i].target == target)
            {

                // Shift remaining listeners left to fill the hole
                for (int j = i; j < count - 1; j++)
                {
                    G_EventSystem.listeners[type][j] = G_EventSystem.listeners[type][j + 1];
                }
                G_EventSystem.listener_counts[type]--;
                return; // Exit once found and removed
            }
        }
    }
}
