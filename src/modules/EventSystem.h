#pragma once
#define MAX_EVENTS 24

typedef struct WindowData
{
    int width;
    int height;
} WindowData;

typedef enum EventType
{
    EVENT_WINDOW_RESIZE,
    EVENT_COUNT,
} EventType;

typedef void (*EventCallback)(void* target, void *data);

void OnEvent(EventType type, void *target, EventCallback callback);
void EmitEvent(EventType type, void *data);
void OffAll(void *target);