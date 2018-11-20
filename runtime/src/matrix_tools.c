#include "matrix_tools.h"
#include "vector_tools.h"

/**
 * Allocates space for a matrix with known type and unknown size
 * @param type
 * @return pointer to matrix
 */
void *getEmptyMatrix(int type){
    matrix * mat = (matrix *) calloc(1, sizeof(matrix *));
    mat->type    = (int *) malloc(sizeof(int*));
    mat->numCols = (int *) malloc(sizeof(int*));
    mat->numRows = (int *) malloc(sizeof(int*));

    *mat->type     = type;
    *mat->numCols  = -1;
    *mat->numRows  = -1;
    mat->elements  = NULL;

    return mat;
}

/**
 * Initializes a matrix with size
 * @param v_matrix
 * @param numRows
 * @param numCols
 */
void initMatrix(void * v_matrix,  int numRows, int numCols){
    assert(numRows >=0 && numCols >=0);

    //cast the matrix
    matrix * mat = (matrix *) v_matrix;

    //assign dimentions
    *mat->numCols = numCols;
    *mat->numRows = numRows;

    //allocate that space
    size_t memberSize = getMemberSize(*mat->type);
    mat->elements = (vector *) calloc((size_t) numRows, memberSize);

    int i = 0;
    for(i = 0; i < numRows; i++){
        *(mat->elements + i) = *(vector *) getEmptyVector(*mat->type);
        initVector(mat->elements + i, numCols);
    }

    setNullMatrix(mat);
}

/**
 * sets the entire matrix to the proper null values
 * @param v_matrix
 */
void setNullMatrix(void * v_matrix){
    matrix * mat = (matrix *) v_matrix;

    int i = 0;
    for(i = 0; i < *mat->numRows; i ++){
        setNullVector(mat->elements + i);
    }
}

/**
 * sets the entire matrix to the proper identity values
 * @param v_matrix
 */
void setIdentityMatrix(void * v_matrix){
    matrix * mat = (matrix *) v_matrix;

    int i = 0;
    for(i = 0; i < *mat->numRows; i ++){
        setIdentityVector(mat->elements + i);
    }
}

/**
 * Gets the row
 * @param v_matrix
 * @return
 */
int getNumRows(void *v_matrix){
    return *((matrix *) v_matrix)->numRows;
}

/**
 * Gets the cols
 * @param v_matrix
 * @return
 */
int getNumCols(void *v_matrix){
    return *((matrix *) v_matrix)->numCols;
}