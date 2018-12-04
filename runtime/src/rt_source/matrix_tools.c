#include "../rt_headers/matrix_tools.h"
#include "../rt_headers/vector_tools.h"

/**
 * Allocates space for a matrix with known type and unknown size
 * @param type
 * @return pointer to matrix
 */
void *getEmptyMatrix(int type){
    matrix * mat = (matrix *) calloc(1, sizeof(matrix));
    mat->type    = (int *) malloc(sizeof(int));
    mat->numCols = (int *) malloc(sizeof(int));
    mat->numRows = (int *) malloc(sizeof(int));

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

    //assign dimensions
    *mat->numCols = numCols;
    *mat->numRows = numRows;

    //allocate that space
    size_t memberSize = getMemberSize(*mat->type);
    mat->elements = (vector *)  calloc((size_t) numRows, sizeof(vector));

    int i = 0;
    for(i = 0; i < numRows; i++){
        *(mat->elements + i) = *(vector *) getEmptyVector(*mat->type);
        initVector(mat->elements + i, numCols);
    }
}

/**
 * destructor for matrix
 * @param v_matrix
 */
void destroyMatrix(void * v_matrix){
    if(v_matrix == NULL)
        return;

    matrix * m = (matrix *) v_matrix;

    //free vectors
    int i = 0;
    vector * v;

    for(i = *m->numRows - 1; i >= 0; i--){
        v = m->elements + i;
        //free type
        if(v->type != NULL)
            free(v->type);

        //free size
        if(v->numElements != NULL)
            free(v->numElements);

        //free elements
        if(v->elements != NULL)
            free(v->elements);
    }

    if(m->type != NULL)
        free(m->type);

    //free numRows
    if(m->numRows != NULL)
        free(m->numRows);

    //free numCols
    if(m->numCols != NULL)
        free(m->numCols);

    //free elements ptr
    if(m->elements != NULL)
        free(m->elements);

    //free matrix
    free(v_matrix);
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
    //get a slice
    vector * slice = sliceScalarVector(v_matrix, scalar, v_vector);

    //copy the slice
    vector * ret = getVectorCopy(slice);

    //discard the slice
    free(slice);

    //return the copy
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
    //get a slice
    vector * slice = sliceVectorScalar(v_matrix, v_vector, scalar);

    //copy the slice
    vector * ret = getVectorCopy(slice);

    //discard the slice
    free(slice);

    //return the copy
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
    //get a slice
    matrix *slice = sliceVectorVector(v_matrix, v_vector_row, v_vector_col);

    //copy the slice
    matrix *ret = getMatrixCopy(slice);

    //free the pointer to the slice old one
    free(slice);

    //return the copy
    return ret;
}

/**
 * print a matrix of any type
 * @param v_matrix
 */
void printMatrix(void *v_matrix){
    //cast the vector
    matrix * mat = (matrix *) v_matrix;

    //loop vars
    int numRows = *mat->numRows;
    int curRow  = 0;
    vector * curVec = mat->elements;


    printf("[");

    for(curRow = 0; curRow < numRows - 1; curRow++){
        printVector(curVec++);
        printf(" ");
    }
    if(numRows)
        printVector(curVec);

    printf("]");
}

/**
 * creates a clone of the matrix and returns it
 * @param v_matrix
 * @return
 */
void *getMatrixCopy(void *v_matrix){
    //cast the null
    matrix * mat = (matrix *) v_matrix;

    //get attibutes for cloning
    int ty = *mat->type;
    int numRows = *mat->numRows;
    int numCols = *mat->numCols;

    //init the return
    matrix * ret = (matrix *) getEmptyMatrix(ty);
    initMatrix(ret, numRows, numCols);

    //loop variables
    int curRow;
    vector * destVec, * srcVec;

    for(curRow = 0; curRow < numRows; curRow++){
        destVec = ret->elements + curRow;
        srcVec  = mat->elements + curRow;
        copyVectorElements(destVec, srcVec);
    }

    //return the copied vector
    return ret;
}


void *sliceScalarVector(void * v_matrix, int scalar, void * v_vector){
    //cast the voids
    matrix * mat = (matrix *) v_matrix;
    vector * vec = (vector *) v_vector;

    //get columns
    int * cols = (int *) vec->elements;

    //init the return
    vector * ret = (vector *) getEmptyVector(*mat->type);
    initVector(ret, *vec->numElements);

    //loop var
    int row = scalar, i = 0, col;
    void *cur, *assign_to;

    //loop and copy the values to the return vector
    for(i = 0; i < *vec->numElements; i++){
        //get column of mat
        col = cols[i];

        //get cell to read from
        cur = getVectorElementPointer(mat->elements + row, col);

        //get cell to assign from
        assign_to = getVectorElementPointer(ret, i);
        assignPointers(assign_to, cur, *ret->type);
    }

    return ret;
}

void *sliceVectorScalar(void * v_matrix, void * v_vector, int scalar){
    //cast the voids
    matrix * mat = (matrix *) v_matrix;
    vector * vec = (vector *) v_vector;

    //get columns
    int * rows = (int *) vec->elements;

    //init the return
    vector * ret = (vector *) getEmptyVector(*mat->type);
    initVector(ret, *vec->numElements);

    //loop var
    int row, i = 0, col = scalar;
    void *cur, *assign_to;

    //loop and copy the values to the return vector
    for(i = 0; i < *vec->numElements; i++){
        //get column of mat
        row = rows[i];

        //get cell to read from
        cur = getVectorElementPointer(mat->elements + row, col);

        //get cell to assign from
        assign_to = getVectorElementPointer(ret, i);
        assignPointers(assign_to, cur, *ret->type);
    }

    return ret;
}

/**
 * Create a new matrix determined by the vector row and col dimensions
 * All pointers in the returned matrix will share with the slice source
 * @param v_matrix
 * @param v_vector_row
 * @param v_vector_col
 * @return
 */
void *sliceVectorVector(void * v_matrix, void * v_vector_row, void * v_vector_col){
    //case the voids
    matrix *mat = (matrix *) v_matrix;
    vector *row_vec = (vector *) v_vector_row;
    vector *col_vec = (vector *) v_vector_col;

    //get rows and cols
    int *rows   = (int *) row_vec->elements;
    int numRows = *row_vec->numElements;
    int *cols   = (int *) col_vec->elements;
    int numCols = *col_vec->numElements;

    //init return
    int type = *mat->type;
    matrix *ret = (matrix *) getEmptyMatrix(type);
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
            left  = getVectorElementPointer(ret->elements +   i, j);
            right = getVectorElementPointer(mat->elements + row, col);
            assignPointers(left, right, type);
        }
    }
    return ret;
}

/**
 * assumes size has been handled
 * @param v_matrix_left
 * @param v_matrix_right
 * @return
 */
void *concatenateMatrices(void * v_matrix_left, void * v_matrix_right){
    //cast the voids
    matrix *left  = (matrix *) v_matrix_left;
    matrix *right = (matrix *) v_matrix_right;

    //get dim and types
    int ty      = *left->type;
    int numRows = *left->numRows;
    int numCols = *left->numCols + *right->numCols;

    //init return
    matrix *ret = (matrix *) getEmptyMatrix(ty);
    initMatrix(ret, numRows, numCols);

    //loop vars
    int i = 0;
    void * v_left_vec, * v_right_vec;
    vector * conc_vec;

    //loop, dont know if this works
    for(i = 0; i < numRows; i++){
        v_left_vec  = left->elements + i;
        v_right_vec = right->elements + i;
        ret->elements[i] = *((vector *) concatenateVectors(v_left_vec, v_right_vec));
    }

    //return
    return ret;
}

void *getIntMatrixMultiplication(void * v_matrix_left, void * v_matrix_right){
    //cast the voids
    matrix * left  = (matrix *) v_matrix_left;
    matrix * right = (matrix *) v_matrix_right;

    //make sure that the size is good
    //assert((*left->numRows == *right->numCols) && (*left->numCols == *right->numRows));

    //get the constants
    int ty = *left->type;
    //int numResultRows = *left->numRows;//(*left->numRows < *right->numCols) ? *left->numRows : *right->numCols;
    //int numResultCols = *left->numRows;//(*left->numCols < *right->numRows) ? *left->numCols : *right->numRows;
    int numResultRows = *left->numRows;//(*left->numRows == 1) ? 1 : *left->numRows;
    int numResultCols = *right->numCols;//(*right->numCols == 1) ? 1 : *left->numCols;

    //init the return
    matrix * ret = (matrix *) getEmptyMatrix(ty);
    initMatrix(ret, numResultRows, numResultCols);

    //loop vars
    int curRow = 0;
    int curCol = 0;
    int i      = 0;
    int *dotResult = (int *) malloc(sizeof(int));
    vector * rightRow, * leftRow;
    void *dest;

    //get a vector to index the right matrix by
    vector * rowIndexVector = (vector *) getEmptyVector(INTEGER);
    initVector(rowIndexVector, *right->numRows);
    for(i = 0; i < *right->numRows; i++){
        ((int *) rowIndexVector->elements)[i] = i;
    }

    //loop
    for(curRow = 0; curRow < numResultRows; curRow++){
        for(curCol = 0; curCol < numResultCols; curCol++){
            //get both pointers
            rightRow   = sliceVectorScalar(right, rowIndexVector, curCol);
            leftRow    = left->elements + curRow;

            //perform dot product
            *dotResult = getIntDotProduct(leftRow, rightRow);

            dest = getVectorElementPointer(ret->elements + curRow,  + curCol);
            assignValFromPointers(dest, dotResult, ty);
        }
    }
    return ret;
}

void *getRealMatrixMultiplication(void * v_matrix_left, void * v_matrix_right){
    //cast the voids
    matrix * left  = (matrix *) v_matrix_left;
    matrix * right = (matrix *) v_matrix_right;

    //make sure that the size is good
    assert((*left->numRows == *right->numCols) && (*left->numCols == *right->numRows));

    //get the constants
    int ty = *left->type;
    int numResultRows = (*left->numRows < *right->numCols) ? *left->numRows : *right->numCols;
    int numResultCols = (*left->numCols < *right->numRows) ? *left->numCols : *right->numRows;

    //init the return
    matrix * ret = (matrix *) getEmptyMatrix(ty);
    initMatrix(ret, numResultRows, numResultCols);

    //loop vars
    int curRow = 0;
    int curCol = 0;
    int i      = 0;
    float *dotResult = (float *) malloc(sizeof(float));
    vector * rightRow, * leftRow;
    void *dest;

    //get a vector to index the right matrix by
    vector * rowIndexVector = (vector *) getEmptyVector(INTEGER);
    initVector(rowIndexVector, *right->numRows);
    for(i = 0; i < *right->numRows; i++){
        ((int *) rowIndexVector->elements)[i] = i;
    }

    //loop
    for(curRow = 0; curRow < numResultRows; curRow++){
        for(curCol = 0; curCol < numResultCols; curCol++){
            //get both pointers
            rightRow   = indexVectorScalar(right, rowIndexVector, curCol);
            leftRow    = left->elements + curRow;

            //perform dot product
            *dotResult = getRealDotProduct(leftRow, rightRow);
            dest = getVectorElementPointer(ret->elements + curRow,  + curCol);
            assignValFromPointers(dest, dotResult, ty);
        }
    }
    return ret;
}

/**
 * assigns a vector to a matrix at the given indices
 * @param v_matrix
 * @param scalar
 * @param v_vector
 * @param v_src
 */
void assignScalarVector(void * v_matrix, int scalar, void * v_vector, void * v_src){
    //cast the voids
    matrix * mat  = (matrix *) v_matrix;
    int      row  = scalar;
    vector * cols = (vector *) v_vector;
    vector * src  = (vector *) v_src;

    //local constants
    int numElements = *src->numElements;
    int ty          = *mat->type;

    //loop vars
    int i   = 0;
    int col = 0;
    void * cur_src, * cur_dest;

    for(i = 0; i < numElements; i++){
        col      = ((int *) cols->elements)[i];
        cur_src  = getVectorElementPointer(src, i);
        cur_dest = getMatrixElementPointer(mat, row, col);
        assignValFromPointers(cur_dest, cur_src, ty);
    }
}

/**
 * assigns a vector to a matrix at the given indices
 * @param v_matrix
 * @param v_vector
 * @param scalar
 * @param v_src
 */
void assignVectorScalar(void * v_matrix, void * v_vector, int scalar, void * v_src){
    //cast the voids
    matrix * mat  = (matrix *) v_matrix;
    int      col  = scalar;
    vector * rows = (vector *) v_vector;
    vector * src  = (vector *) v_src;

    //local constants
    int numElements = *src->numElements;
    int ty          = *mat->type;

    //loop vars
    int i   = 0;
    int row = 0;
    void * cur_src, * cur_dest;

    for(i = 0; i < numElements; i++){
        row      = ((int *) rows->elements)[i];
        cur_src  = getVectorElementPointer(src, i);
        cur_dest = getMatrixElementPointer(mat, row, col);
        assignValFromPointers(cur_dest, cur_src, ty);
    }
}

/**
 * assigns matrix to another matrix with the given indices
 * @param v_matrix
 * @param v_vector_row
 * @param v_vector_col
 * @param v_src
 */
void assignVectorVector(void * v_matrix, void * v_vector_row, void * v_vector_col, void * v_src){
    //cast the voids
    matrix * mat = (matrix *) v_matrix;
    matrix * src = (matrix *) v_src;
    int * rows   = (int *) ((vector *) v_vector_row)->elements;
    int * cols   = (int *) ((vector *) v_vector_col)->elements;

    //local constants
    int numRows = *src->numRows;
    int numCols = *src->numCols;
    int ty      = *mat->type;

    //loop vars
    int curCol, curRow;
    void * src_p, * dest_p;
    int i,j;

    //loop
    for(i = 0; i < numRows; i++){
        for(j = 0; j < numCols; j++){
            //get both pointers
            curRow = rows[i];
            curCol = cols[j];

            dest_p = getMatrixElementPointer(mat, curRow, curCol);
            src_p  = getMatrixElementPointer(src, i, j);
            assignValFromPointers(dest_p, src_p, ty);
        }
    }
}

/**
 * Copies the right side elements to the left side, truncates and nullpads when necessary
 * @param m_dest
 * @param m_src
 */
void copyMatrixElements(void *m_dest, void *m_src) {
    //Cast the voids
    matrix *dest = (matrix *) m_dest;
    matrix *src = (matrix *) m_src;

    //Loop variables
    int i;
    void *v_src, *v_dest;
    int destRow = *dest->numRows;

    for (i = 0; i < destRow; i++) {
        // Get vector at row i in matrix
        v_src = src->elements + i;
        v_dest = dest->elements + i;

        copyVectorElements(v_dest, v_src);
    }
}


/**
 * Copies the right side elements to the left side, only if size the same.
 * @param m_dest
 * @param m_src
 * @param line
 * @param assTruePromoFalse
 */
void strictCopyMatrixElements(void *m_dest, void *m_src, int line, int assTruePromoFalse) {
    //Cast the voids
    matrix *dest = (matrix *) m_dest;
    matrix *src = (matrix *) m_src;

    //Values
    char *srcType = getType(*src->type);
    char *destType = getType(*dest->type);

    int srcRow = *src->numRows;
    int destRow = *dest->numRows;

    int srcCol = *src->numCols;
    int destCol = *dest->numCols;

    //Loop variables
    int i;
    void *v_src, *v_dest;

    if (((*dest->numRows != *src->numRows || *dest->numCols != *src->numCols) && assTruePromoFalse == false) ||
            ((*dest->numRows < *src->numRows || *dest->numCols < *src->numCols) && assTruePromoFalse == true)) {
        printf("Type error: Cannot convert between %s matrix[%d,%d] and %s matrix[%d,%d] on line %d\n", srcType, srcRow,
               srcCol, destType, destRow, destCol, line);
        exit(1);
    } else {
        for (i = 0; i < destRow; i++) {
            // Get vector at row i in matrix
            v_src = src->elements + i;
            v_dest = dest->elements + i;

            strictCopyVectorElements(v_dest, v_src, line, assTruePromoFalse);
        }
    }
}

/**
 * copies a matrix
 * @param v_matrix
 * @return
 */
void *copyMatrix(void * v_matrix){
    //cast the voids
    matrix * mat = (matrix *) v_matrix;

    //locals
    int ty      = *mat->type;
    int numRows = *mat->numRows;
    int numCols = *mat->numCols;

    //init the return
    matrix * ret = (matrix *) getEmptyMatrix(ty);
    initMatrix(ret, numRows, numCols);

    //loop var
    void * cur_dest, * cur_src;
    int i = 0;

    //loop
    for(i = 0; i < numRows; i++){
        cur_dest = ret->elements + i;
        cur_src  = mat->elements + i;
        copyVectorElements(cur_dest, cur_src);
    }

    return ret;
}

void resizeMatrix(void * v_matrix, int newNumRows, int newNumCols){
    //cast the void
    matrix * mat = (matrix *) v_matrix;

    //local constants
    int prevNumRows = *mat->numRows;
    int prevNumCols = *mat->numCols;

    //loop vars
    int i = 0;
    vector * curVec;

    for(i = 0; i < prevNumRows; i++){
        curVec = mat->elements + i;
        resizeVector(curVec, newNumCols);
    }

    //set the matrix size
    *mat->numRows = newNumRows;
    *mat->numCols = newNumCols;
}