#pragma once

typedef struct SolConfig
{
    char name[50];
    float volume, opacity;
    float lookSens;
} SolConfig;

extern SolConfig LocalConfig;