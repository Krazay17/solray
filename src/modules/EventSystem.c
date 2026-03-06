#include "EventSystem.h"
#include <stdio.h>

#define MAX_LISTENERS 10

typedef struct {
    EventCallback listeners[MAX_EVENTS][MAX_LISTENERS];
    int listener_counts[MAX_EVENTS];
} EventSystem;

// Our single global instance
static EventSystem G_EventSystem = {0};


void OnEvent(EventType type, EventCallback callback)
{
    int count = G_EventSystem.listener_counts[type];
    if(count < MAX_LISTENERS)
    {
        G_EventSystem.listeners[type][count] = callback;
        G_EventSystem.listener_counts[type]++;
    }
}

void EmitEvent(EventType type, void *data)
{
    int count = G_EventSystem.listener_counts[type];
    for(int i =0;i<count;i++)
    {
        G_EventSystem.listeners[type][i](data);
    }
}
