#pragma once

#include "rt_globals.h"

//init related
void *getEmptyMatrix(int type);
void  initMatrix(void * v_matrix,  int numRows, int numCols);
void  setNullMatrix(void * v_matrix);
void  setIdentityMatrix(void * v_matrix);

//indexing
void *indexScalarVector(void * v_matrix, int scalar, void * v_vector);
void *indexVectorScalar(void * v_matrix, void * v_vector, int scalar);
void *indexVectorVector(void * v_matrix, void * v_vector_row, void * v_vector_col);

//built ins
int getNumRows(void *v_matrix);
int getNumCols(void *v_matrix);

//helpers
void *getMatrixElementPointer(void *v_matrix, int row, int col);