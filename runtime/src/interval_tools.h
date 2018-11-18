#pragma once

#include "rt_globals.h"

void *getNewInterval(int lower, int upper);
void *getVectorFromInterval(void * v_interval, int by);