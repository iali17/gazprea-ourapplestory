#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

// Add function named dummyPrint with signature void(int) to llvm to have this linked in.
void dummyPrint(int i) {
  printf("I'm a function! %d\n", i);
}

/**
 * Creates a vector with set size and type
 * @param type
 * @param numElements
 * @return
 */
vector *getVector(int type, int numElements){
    //get space
    assert(numElements > 0);
    unsigned int numElm = (unsigned int) numElements;
    vector* ret      = (vector *) calloc(1, sizeof(vector *));
    ret->type        = (int *) malloc(sizeof(int *));
    ret->numElements = (int *) malloc(sizeof(int *));
    ret->elements    = calloc(numElm, sizeof(void *));

    //fill
    *ret->type        = type;
    *ret->numElements = numElements;
    return ret;
}

/**
 * Creates a vector with known type and uknown size
 * @param type
 * @return
 */
vector *getEmptyVector(int type){
    vector* ret      = (vector *) calloc(1, sizeof(vector *));
    ret->type        = (int *) malloc(sizeof(int *));
    ret->numElements = (int *) malloc(sizeof(int *));


    return ret;
}

void setVal(int idx, int type, void *l, void *r){
    if(type == 1){
        
    }
}

void setNull(int idx, int type, void *l){

}

vector *getVectorFromVector(int type, int numElements, vector *old_vector){
    vector *ret = getEmptyVector(type);
    if (numElements >= 0){
        *ret->numElements = numElements;
        assert(*ret->numElements >= *old_vector->numElements);
    } else {
        *ret->numElements = old_vector->numElements;
    }

    ret->elements = calloc(*ret->numElements, sizeof(void *));

    int  i;
    for(i = 0; i < *ret->numElements; i++)
        if(i < *old_vector->numElements)
            ((int *) ret->elements)[i] = ((int *) old_vector->elements)[i];
        else
            ((int *) ret->elements)[i] = 0;

}

void setVectorElement(int idx, void* val){

}