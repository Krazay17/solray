#pragma once
#define MAX_EVENTS 24

typedef enum EventType
{
    EVENT_WINDOW_RESIZE,
    EVENT_COUNT,
} EventType;

typedef void (*EventCallback)(void *data);

void OnEvent(EventType type, EventCallback callback);
void EmitEvent(EventType type, void *data);