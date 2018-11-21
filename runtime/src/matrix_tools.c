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

/**
 * Gets the pointer to a matrix element
 * @param v_matrix
 * @param row
 * @param col
 * @return
 */
void *getMatrixElementPointer(void *v_matrix, int row, int col){
    matrix * mat = (matrix *) v_matrix;
    vector * vec = mat->elements + row;
    return getVectorElementPointer(vec, col);
}

/**
 * For accessing a matrix by scalar, vector. Assumes ranges have been dealt with
 * @param v_matrix
 * @param scalar
 * @param v_vector
 * @return - vector
 */
void *indexScalarVector(void * v_matrix, int scalar, void * v_vector){
    //cast the voids
    matrix * mat = (matrix *) v_matrix;
    vector * vec = (vector *) v_vector;

    //get columns
    int * cols = (int *) vec->elements;

    //init the return
    matrix * ret = getEmptyMatrix(*mat->type);
    initMatrix(ret, scalar, *vec->numElements);

    //loop var
    int row = scalar, i = 0, col;
    void *cur, *assign_to;

    //loop and copy the values to the return vector
    for(i = 0; i < *vec->numElements; i++){
        //get column of mat
        col = cols[i];

        //get cell to read from
        cur = (mat->elements + row)->elements + col;

        //get cell to assign from
        assign_to = (ret->elements + row) + i;
        assignValFromPointers(assign_to, cur, *ret->type);
    }

    return ret;
}

/**
 * For accessing matrix by vector, scalar. Assumes ranges have been dealt with
 * @param v_matrix
 * @param v_vector
 * @param scalar
 * @return - vector
 */
void *indexVectorScalar(void * v_matrix, void * v_vector, int scalar){
    //cast the voids
    matrix * mat = (matrix *) v_matrix;
    vector * vec = (vector *) v_vector;

    //get columns
    int * rows = (int *) vec->elements;

    //init the return
    matrix * ret = getEmptyMatrix(*mat->type);
    initMatrix(ret, scalar, *vec->numElements);

    //loop var
    int row, i = 0, col= scalar;
    void *cur, *assign_to;

    //loop and copy the values to the return vector
    for(i = 0; i < *vec->numElements; i++){
        //get column of mat
        row = rows[i];

        //get cell to read from
        cur = (mat->elements + row)->elements + col;

        //get cell to assign from
        assign_to = (ret->elements + i) + col;
        assignValFromPointers(assign_to, cur, *ret->type);
    }

    return ret;
}

/**
 * For accessing matrix by vector, vector. Assumes ranges have been dealt with
 * @param v_matrix
 * @param v_vector_row
 * @param v_vector_col
 * @return - matrix
 */
void *indexVectorVector(void * v_matrix, void * v_vector_row, void * v_vector_col) {
    //case the voids
    matrix *mat = (matrix *) v_matrix;
    vector *row_vec = (vector *) v_vector_row;
    vector *col_vec = (vector *) v_vector_col;

    //get rows and cols
    int *rows = (int *) row_vec->elements;
    int numRows = *row_vec->numElements;
    int *cols = (int *) col_vec->elements;
    int numCols = *col_vec->numElements;

    //init return
    int type = *mat->type;
    matrix *ret = getEmptyMatrix(type);
    initMatrix(ret, numRows, numCols);

    //loop vars
    int i, j;
    int row, col;
    void *left, *right;

    //assign new matrix
    for (i = 0; i < numRows; i++){
        row = rows[i];
        for (j = 0; j < numCols; j++) {
            col   = cols[j];
            left  = (ret->elements +   i)->elements + j;
            right = (mat->elements + row)->elements + col;
            assignValFromPointers(left, right, type);
        }
    }
    return ret;
}