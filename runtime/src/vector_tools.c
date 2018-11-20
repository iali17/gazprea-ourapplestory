#include "vector_tools.h"

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

/**
 * set a value of a vector at a given index to the value at the given pointer
 * @param v
 * @param idx
 * @param val
 */
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

/**
 * gets the value of the null and stores it in the return pointer
 * @param type
 * @param ret
 * @return
 */
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

/**
 * gets the value of the identity and stores it in the return pointer
 * @param type
 * @param ret
 * @return
 */
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


void * getCast(void * v_p, int type, void *ret){
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

/**
 * sets the given vector to be the null vector
 * @param v_vector
 */
void setNullVector(void * v_vector){
    //cast the vector
    vector * vec = (vector *) v_vector;

    //get the null value
    void *my_null = malloc(sizeof(int));
    getNull(*vec->type, my_null);

    //set the vector to null
    int i;
    for(i = 0; i < *vec->numElements; i++){
        setVectorVal(vec, i, my_null);
    }

    free(my_null);
}

/**
 * sets a given vector to be the idenity
 * @param v_vector
 */
void setIdentityVector(void * v_vector){
    //cast the vector
    vector * vec = (vector *) v_vector;

    //get the identity value
    void *my_identity = malloc(sizeof(int));
    getIdentity(*vec->type, my_identity);

    //set the vector to identity
    int i;
    for(i = 0; i < *vec->numElements; i++){
        setVectorVal(vec, i, my_identity);
    }

    free(my_identity);
}

/**
 * get the size of a member
 * @param type
 * @return
 */
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

/**
 * gets the length of a vector
 * @param v_vector
 * @return
 */
int getVectorLength(void * v_vector){
    return *((vector *) v_vector)->numElements;
}

/**
 * initalizes the vector to have a size and sets all of the members to null
 * @param v_vector
 * @param numMem
 */
void initVector(void * v_vector,  int numMem){
    assert(numMem >= 0);
    //cast it
    vector * vec = (vector *) v_vector;

    //allocate that space
    size_t memberSize = getMemberSize(*vec->type);
    *vec->numElements = numMem;
    vec->elements = calloc((size_t) numMem, memberSize);

    //set it to its null
    setNullVector(vec);
}

/**
 * get a pointer to an element in a vector at the given index
 * @param v_vector
 * @param idx
 * @return
 */
void *getVectorElementPointer(void *v_vector, int idx){
    vector * v = (vector *) v_vector;

    if(*v->type == BOOLEAN){
        return ((bool *) v->elements)  + idx;
    }
    else if (*v->type == CHAR){
        return ((char *) v->elements)  + idx;
    }
    else if (*v->type == INTEGER){
        return ((int *) v->elements)   + idx;
    }
    else if (*v->type == REAL){
        return ((float *) v->elements) + idx;
    }
    return NULL;
}

/**
 * return null value if out of bounds
 * @param v_vector
 * @param idx
 * @return
 */
void  *getVectorElementPointerSafe(void *v_vector, int idx){
    if (validIndex(v_vector, idx))
        return getVectorElementPointer(v_vector, idx);
    else {
        void *ret = malloc(sizeof(int));
        getNull(*((vector *) v_vector)->type, ret);
        return ret;
    }
}

bool validIndex(void *v_vector, int idx){
    return ((0 <= idx) && (idx < *((vector *) v_vector)->numElements));
}

/**
 *
 * @param v_vector
 * @return
 */
void *getReverseVector(void * v_vector){
    //cast vector
    vector * from_vector = (vector *) v_vector;

    //init return
    vector * ret         = getEmptyVector(*from_vector->type);
    initVector(ret, *from_vector->numElements);

    //sets the return to contain the reverse
    int upper = *from_vector->numElements - 1;
    int i;
    for(i = 0; i <= upper; i++){
        setVectorVal(ret, upper - 1 - i, getVectorElementPointer(from_vector, i));
    }

    return ret;
}

/**
 * Take a pointer to a vector elements and print the value
 * @param v_elm_ptr
 * @param type
 */
void printVectorElement(void *v_elm_ptr, int type){
    if(type == BOOLEAN){
        if (*((bool *) v_elm_ptr) == true)
            printf("T");
        else
            printf("F");
    }
    else if (type == CHAR){
        printf("%c", *((char *) v_elm_ptr));
    }
    else if (type == INTEGER){
        printf("%d", *((int *) v_elm_ptr));
    }
    else if (type == REAL){
        printf("%g", *((float *) v_elm_ptr));
    }
}

/**
 * Print all of the elements of a vector
 * @param v_vector
 */
void printVector(void* v_vector){
    //cast to vector
    vector * vec = (vector *) v_vector;

    //local vars
    int type = *vec->type;
    int i    = 0;
    void *element_pointer;

    //print vector
    for(i = 0; i < *vec->numElements - 1; i++){
        element_pointer = getVectorElementPointer(vec, i);
        printVectorElement(element_pointer, type);
        printf(" ");
    }
    element_pointer = getVectorElementPointer(vec, i);
    printVectorElement(element_pointer, type);
}