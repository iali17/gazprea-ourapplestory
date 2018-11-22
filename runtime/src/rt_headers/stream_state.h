#pragma once

#include "rt_globals.h"

int get_stream_state(void * v_stream);
void read_in(void * v_stream, void * v_dest, int type);