#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>

#include "structures.h"

struct TSVData {
    Matrix* matrix;
    RingBuffer iter_data;
};

TSVData readTSV(char* input_file);

void saveBMP(char* path, Matrix* data);

TSVData readBMP(char* input_file);