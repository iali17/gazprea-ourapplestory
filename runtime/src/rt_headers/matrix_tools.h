#pragma once

#include "rt_globals.h"

//init related
void *getEmptyMatrix(int type);
void  initMatrix(void * v_matrix,  int numRows, int numCols);
void  setNullMatrix(void * v_matrix);
void  setIdentityMatrix(void * v_matrix);
void  destroyMatrix(void * v_matrix);

//indexing
void *indexScalarVector(void * v_matrix, int scalar, void * v_vector);
void *indexVectorScalar(void * v_matrix, void * v_vector, int scalar);
void *indexVectorVector(void * v_matrix, void * v_vector_row, void * v_vector_col);

//slice functions
void *sliceScalarVector(void * v_matrix, int scalar, void * v_vector);
void *sliceVectorScalar(void * v_matrix, void * v_vector, int scalar);
void *sliceVectorVector(void * v_matrix, void * v_vector_row, void * v_vector_col);

//copy related
void *getMatrixCopy(void *v_matrix);

//built ins
int  getNumRows(void *v_matrix);
int  getNumCols(void *v_matrix);
void printMatrix(void *v_matrix);
void *concatenateMatrices(void * v_matrix_left, void * v_matrix_right);
void *getIntMatrixMultiplication(void * v_matrix_left, void * v_matrix_right);
void *getRealMatrixMultiplication(void * v_matrix_left, void * v_matrix_right);

//helpers
void *getMatrixElementPointer(void *v_matrix, int row, int col);