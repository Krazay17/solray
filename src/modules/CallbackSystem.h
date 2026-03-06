#pragma once
#define MAX_CALLBACKS 24

void *callbacks[MAX_CALLBACKS];

void OnCallback(const char *name, void *callback);
void Callback_Do();
void OffCallback(const char *name, void *callback);