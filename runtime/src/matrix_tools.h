#pragma once

#include "rt_globals.h"

//init related
void *getEmptyMatrix(int type);
void  initMatrix(void * v_matrix,  int numRows, int numCols);
void  setNullMatrix(void * v_matrix);
void  setIdentityMatrix(void * v_matrix);

//indexing

//built ins
int getNumRows(void *v_matrix);
int getNumCols(void *v_matrix);