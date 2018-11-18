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
 * Creates a vector with known type and unknown size
 * @param type
 * @return
 */
void *getEmptyVector(int type){
    vector* ret      = (vector *) calloc(1, sizeof(vector *));
    ret->type        = (int *) malloc(sizeof(int *));
    ret->numElements = (int *) malloc(sizeof(int *));

    *ret->type        = type;
    *ret->numElements = -1;
    ret->elements     = NULL;
    return ret;
}

void setVectorVal(vector * v, int idx, void *val){
    if(*v->type == BOOLEAN){
        ((bool *) v->elements)[idx] = *((bool *) val);
    }
    else if (*v->type == CHAR){
        ((char *) v->elements)[idx] = *((char *) val);
    }
    else if (*v->type == INTEGER){
        ((int *) v->elements)[idx] = *((int *) val);
    }
    else if (*v->type == REAL){
        ((float *) v->elements)[idx] = *((float *) val);
    }
}

void * getNull(int type, void *ret){
    if(type == BOOLEAN){
        *((bool *) ret) =  false;
    }
    else if (type == CHAR){
        *((char *) ret) =  (char) 0;
    }
    else if (type == INTEGER){
        *((int *) ret) = 0;
    }
    else if (type == REAL){
        *((float *) ret) =  0.0;
    }
    return ret;
}

void * getIdentity(int type, void *ret){
    if(type == BOOLEAN){
        *((bool *) ret) = true;
    }
    else if (type == CHAR){
        *((char *) ret) = 0x01;
    }
    else if (type == INTEGER){
        *((int *) ret) = 1;
    }
    else if (type == REAL){
        *((float *) ret) =  1.0;
    }
    return ret;
}

void setNullVector(void * v_vector){
    //cast the vector
    vector * vec = (vector *) v_vector;

    //get the null value
    void *myNull = malloc(sizeof(int));
    getNull(*vec->type, myNull);

    //set the vector to null
    int i;
    for(i = 0; i < *vec->numElements; i++){
        setVectorVal(vec, i, myNull);
    }

    free(myNull);
}

void setIdentityVector(void * v_vector){
    //cast the vector
    vector * vec = (vector *) v_vector;

    //get the identity value
    void *myIdentity = malloc(sizeof(int));
    getIdentity(*vec->type, myIdentity);

    //set the vector to identity
    int i;
    for(i = 0; i < *vec->numElements; i++){
        setVectorVal(vec, i, myIdentity);
    }

    free(myIdentity);
}

size_t getMemberSize(int type){
    size_t memberSize = 0;

    if(type == BOOLEAN){
        memberSize = sizeof(bool);
    }
    else if (type == CHAR){
        memberSize = sizeof(char);
    }
    else if (type == INTEGER){
        memberSize = sizeof(int);
    }
    else if (type == REAL){
        memberSize = sizeof(float);
    }
    else {
        exit(1);
    }
    return memberSize;
}

void initVector(void * v_vector,  int numMem){
    assert(numMem > 0);
    //cast it
    vector * vec = (vector *) v_vector;

    //allocate that space
    size_t memberSize = getMemberSize(*vec->type);
    *vec->numElements = numMem;
    vec->elements = calloc((size_t) numMem, memberSize);

    //set it to its null
    setNullVector(vec);
}