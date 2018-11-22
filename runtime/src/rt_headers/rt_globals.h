#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../../../include/globals.h"

/**
 * Predefined macro for string. Has the same form as a vector so that the same operations can apply
 */
typedef struct{
    int  *type;
    int  *length;
    char *elements;
} string;

/**
 * Definition of a generic vector. Element type is a generalized to void ptr
 */
typedef struct{
    int  *type;
    int  *numElements;
    void *elements;
} vector;

/**
 * Definition of a generic matrix. Element type is a generic vector
 */
typedef struct{
    int     *type;
    int     *numRows;
    int     *numCols;
    vector  *elements;
} matrix;

/**
 * Definition of an interval. This matches our llvm declaration
 */
typedef struct{
    int *upper;
    int *lower;
} interval;

/**
 * stream state boy
 */
typedef struct{
    int stream_type;
    int stream_state;
} stream;