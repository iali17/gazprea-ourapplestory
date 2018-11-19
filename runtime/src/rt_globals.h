#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../../include/globals.h"

typedef struct{
    int  *type;
    int  *numElements;
    void *elements;
} vector;

typedef struct{
    int     *type;
    int     *numRows;
    int     *numCols;
    vector **elements;
} matrix;

typedef struct{
    int *upper;
    int *lower;
} interval;