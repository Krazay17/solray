#pragma once
#include <stdio.h>
#include "GlobalState.h"

extern FILE configFile;

SolConfig Load_Config(void);

void Save_Config(SolConfig *config);