#pragma once

typedef struct SolConfig
{
    char name[50];
    float volume, opacity;
} SolConfig;

extern SolConfig LocalConfig;