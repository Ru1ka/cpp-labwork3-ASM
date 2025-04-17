#pragma once
#include "IO_manager.h"
#include "scripts.h"
#include "structures.h"

void makeIteration(Matrix* data);

void iterateLoop(Matrix* data, RingBuffer* cur_iteration_data, Args CLI_data);
