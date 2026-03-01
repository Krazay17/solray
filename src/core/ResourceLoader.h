#pragma once
// Do NOT include windows.h or raylib.h here
typedef struct {
    void* data;
    unsigned int size;
} RawResource;

RawResource LoadResourceData(const char* resourceName);