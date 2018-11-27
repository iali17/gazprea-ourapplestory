#include "../rt_headers/vector_tools.h"

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

    //local constants
    int ty = *from_vector->type;

    //init return
    vector * ret = (vector *) getEmptyVector(ty);
    initVector(ret, *from_vector->numElements);

    //sets the return to contain the reverse
    int upper = *from_vector->numElements - 1;
    int i;
    void *l, *r;
    for(i = upper; i >= 0; i--){

        l = getVectorElementPointer(ret, upper - i);
        r = getVectorElementPointer(from_vector, i);
        assignValFromPointers(l, r, ty);
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

    printf("[");

    //print vector
    for(i = 0; i < *vec->numElements - 1; i++){
        element_pointer = getVectorElementPointer(vec, i);
        printVectorElement(element_pointer, type);
        printf(" ");
    }
    //only print if valid
    if(*vec->numElements > 0) {
        element_pointer = getVectorElementPointer(vec, i);
        printVectorElement(element_pointer, type);
    }

    printf("]");
}

void printVectorAsString(void * v_vector){
	//cast the void
	vector * vec = (vector *) v_vector;

	//get the string
	char * str = (char *) vec->elements;

	//print it
	printf("%s", str);
}

/**
 * Assigns value pointed to by right hand side to the left hand side
 * @param l
 * @param r
 * @param type
 */
void assignValFromPointers(void *l, void *r, int type){
    if(type == BOOLEAN)
        *((bool *) l) = *((bool *) r);
    else if (type == CHAR)
        *((char *) l) = *((char *) r);
    else if (type == INTEGER)
        *((int *) l) = *((int *) r);
    else if (type == REAL)
        *((float *) l) = *((float *) r);
}

/**
 * make the left hand a side a copy of the right hand side pointer
 * @param l
 * @param r
 * @param type
 */
void assignPointers(const void *l, const void *r, int type){
    if(type == BOOLEAN)
        *((bool *) l) = *((bool *) r);
    else if (type == CHAR)
        *((char *) l) = *((char *) r);
    else if (type == INTEGER)
        *((int *) l) = *((int *) r);
    else if (type == REAL)
        *((float *) l) = *((float *) r);
}

/**
 * Copies the right side elements to the left side. If RHS is shorter than LHS padding will occur
 * @param v_dest
 * @param v_src
 */
void copyVectorElements(void *v_dest, void *v_src){
	//cast the voids
	vector * dest = (vector *) v_dest;
	vector * src  = (vector *) v_src;
	
	//loop vars
	int i = 0;
	int ty = *dest->type;
	int numElements = *dest->numElements;
	void *src_p, *dest_p;

	//copy values. note that we load safe from the source vector
	for(i = 0; i < numElements; i++){
		src_p  = getVectorElementPointerSafe(src, i);
		dest_p = getVectorElementPointer(dest,i);
		assignValFromPointers(dest_p, src_p, ty);
	}
}

void strictCopyVectorElements(void *v_dest, void *v_src, int line){
    //cast the voids
    vector * dest = (vector *) v_dest;
    vector * src  = (vector *) v_src;

    if(*dest->numElements != *src->numElements) {
        char *srcType = getType(*src->type);
        char *destType = getType(*dest->type);
        int  srcIndex = *src->numElements;
        int  destIndex = *dest->numElements;

        printf("Type error: Cannot convert between %s vector[%d] and %s vector[%d] on line %d", srcType, srcIndex, destType, destIndex, line);
        exit(1);
    } else {
        copyVectorElements(v_dest, v_src);
    }
}


/**
 * Returns a slice of the vector. The returned vector will share the same pointers as the argument vector
 * @param v_vector
 * @param v_index
 * @return
 */
void *getVectorSlice(void *v_vector, void *v_index){
	//cast the void. Not we will assume idx is a int vector
	vector * vec     = (vector *) v_vector;
	vector * idx_vec = (vector *) v_index;

	//get return info
	int ty = *vec->type;
	int numElements = *idx_vec->numElements;

	//init return
	vector * ret = (vector *) getEmptyVector(ty);
	initVector(ret, numElements);

	//loop vars
	int i, idx;
	void *src_p, *dest_p;

	//copy pointers
	for(i = 0; i < numElements; i++){
		idx = ((int *) idx_vec->elements)[i];
		src_p  = getVectorElementPointer(vec, idx);
        dest_p = getVectorElementPointer(ret, i);
        assignPointers(dest_p, src_p, ty);
	}

	//return
	return ret;
}

/**
 * clone a vector
 * @param v_vector
 * @return - clone
 */
void *getVectorCopy(void *v_vector){
	//cast the void
	vector * src = (vector *) v_vector;
	
	//init the return
	vector * ret = (vector *) getEmptyVector(*src->type);
	initVector(ret, *src->numElements);
	
	//copy
	copyVectorElements(ret, src);

	//return
	return ret;
}

/**
 * assigns to the destination vector from the source at the given indices
 * @param v_vec_dest
 * @param v_vec_idx
 * @param v_vec_src
 */
void assignFromVector(void * v_vec_dest, void * v_vec_idx, void * v_vec_src){
    //cast the voids
    vector * dest = (vector *) v_vec_dest;
    vector * idx  = (vector *) v_vec_idx;
    vector * src  = (vector *) v_vec_src;

    //local constants
    int ty     = *dest->type;
    int *idxP  = (int *) idx->elements;
    int idxLen = *idx->numElements;

    //loop vars
    int dest_idx = 0;
    int i        = 0;
    void *cur_src;
    void *cur_dest;

    //copy
    for(i = 0; i < idxLen; i++){
        dest_idx = idxP[i];
        cur_src  = getVectorElementPointer(src, i);
        cur_dest = getVectorElementPointer(dest, dest_idx);
        assignValFromPointers(cur_dest, cur_src, ty);
    }
}

/**
 * concatenates both vectors into a new one
 * @param v_vector_left
 * @param v_vector_right
 * @return
 */
void *concatenateVectors(void * v_vector_left, void * v_vector_right) {
    //cast the voids
    vector *left  = (vector *) v_vector_left;
    vector *right = (vector *) v_vector_right;

    //get variables for the return vector
    int numLeftElements  = *left->numElements;
    int numRightElements = *right->numElements;
    int numElements      = numLeftElements + numRightElements;
    int ty               = *left->type;

    //init ret
    vector * ret = getEmptyVector(ty);
    initVector(ret, numElements);

    //loop vars
    int i = 0;
    void * v_dest, * v_src;

    //copy the left
    for(i = 0; i < numLeftElements; i++){
        v_dest = getVectorElementPointer(ret, i);
        v_src  = getVectorElementPointer(left, i);
        assignValFromPointers(v_dest, v_src, ty);
    }

    //copy the right
    for(i = 0; i < numRightElements; i++){
        v_dest = getVectorElementPointer(ret, numLeftElements + i);
        v_src  = getVectorElementPointer(right, i);
        assignValFromPointers(v_dest, v_src, ty);
    }

    //return it
    return ret;
}

/**
 * returns pointer to result
 * @param v_vector_left
 * @param v_vector_right
 * @return
 */
void *dotProduct(void * v_vector_left, void * v_vector_right){
    //cast the voids
    vector * left  = (vector *) v_vector_left;
    vector * right = (vector *) v_vector_right;

    //get constants
    int ty = *left->type;

    //init return
    void * ret = malloc(sizeof(int));

    assert((ty == INTEGER) || (ty == REAL));

    if(ty == INTEGER){
        int res = getIntDotProduct(left, right);
        *((int *) ret) = res;
    }
    else {
        float res = getRealDotProduct(left, right);
        *((float *) ret) = res;
    }

    return ret;
}

/**
 * does the dot product on an int vector
 * @param v_vector_left
 * @param v_vector_right
 * @return
 */
int getIntDotProduct(void * v_vector_left, void * v_vector_right){
    //cast the voids
    vector * left  = (vector *) v_vector_left;
    vector * right = (vector *) v_vector_right;

    //local vars
    int numElements = (*right->numElements > *left->numElements) ?  *right->numElements : *left->numElements;

    //init return
    int ret = 0;

    //loop vars
    int i = 0;
    int l_op, r_op;

    //perform operation
    for(i = 0; i < numElements; i++){
        l_op = *((int *) getVectorElementPointerSafe(left,  i));
        r_op = *((int *) getVectorElementPointerSafe(right, i));
        ret += l_op + r_op;
    }

    //return
    return ret;
}

/**
 * does the dot product on a real vector
 * @param v_vector_left
 * @param v_vector_right
 * @return
 */
float getRealDotProduct(void * v_vector_left, void * v_vector_right){
    //cast the voids
    vector * left  = (vector *) v_vector_left;
    vector * right = (vector *) v_vector_right;

    //local vars
    int numElements = (*right->numElements > *left->numElements) ?  *right->numElements : *left->numElements;

    //init return
    float ret = 0;

    //loop vars
    int i = 0;
    float l_op, r_op;

    //perform operation
    for(i = 0; i < numElements; i++){
        l_op = *((float *) getVectorElementPointerSafe(left,  i));
        r_op = *((float *) getVectorElementPointerSafe(right, i));
        ret += l_op + r_op;
    }

    //return
    return ret;
}

/**
 * just returns an empty vector with the proper result size
 * @param v_vector_left
 * @param v_vector_right
 * @return
 */
void *getOpResultVector(void * v_vector_left, void * v_vector_right){
    //cast the voids
    vector * left  = (vector *) v_vector_left;
    vector * right = (vector *) v_vector_right;

    //get constants
    int ty = *left->type;
    int numElements = (*right->numElements > *left->numElements) ?  *right->numElements : *left->numElements;

    //init return
    vector * ret = (vector *) getEmptyVector(ty);
    initVector(ret, numElements);

    return ret;
}

/**
 * gets the string type
 * @param type
 * @return
 */
char *getType(int type) {
    if(type == BOOLEAN)
        return "boolean";
    else if (type == CHAR)
        return "character";
    else if (type == INTEGER)
        return "integer";
    else if (type == REAL)
        return "real";
}

/**
 * performs 'by' operation on vectors of any type
 * @param v_vector
 * @param by
 * @return
 */
void *getVectorBy(void * v_vector, int by){
    //cast the voids
    vector * vec = (vector *) v_vector;

    //local constants
    int ty = *vec->type;

    //prepare the return
    vector * ret       = (vector *) getEmptyVector(ty);
    int numElements = *vec->numElements;
    initVector(ret, numElements);

    //loop vars
    int i = 0, j = 0;
    void * left;
    void * right;

    for(i = 0; i < numElements; i+=by){
        left  = getVectorElementPointer(ret, j);
        right = getVectorElementPointer(vec, i);
        assignValFromPointers(left, right, ty);
        j++;
    }

    //fix size
    *ret->numElements = j;

    //return
    return ret;
}