#pragma once

#include "rt_globals.h"

//init related
void *getEmptyVector(int type);
void  initVector(void * v_vector,  int numMem);
void  setNullVector(void * v_vector);
void  setIdentityVector(void * v_vector);

//print related
void printVector(void* v_vector);
void printVectorElement(void *v_elm_ptr, int type);

//built ins
int   getVectorLength(void * v_vector);
void *getReverseVector(void * v_vector);
void *concatenateVectors(void * v_vector_left, void * v_vector_right);

//helpers
size_t getMemberSize(int type);
void   setVectorVal(vector * v, int idx, void *val);
void  *getVectorElementPointer(void *v_vector, int idx);
void  *getVectorElementPointerSafe(void *v_vector, int idx);
bool   validIndex(void *v_vector, int idx);
void  *getNull(int type, void *ret);
void  *getIdentity(int type, void *ret);
char  *getType(int type);

//assignment
void assignValFromPointers(void *l, void *r, int type);
void assignPointers(void *l, void *r, int type);
void copyVectorElements(void *v_dest, void *v_src);
void strictCopyVectorElements(void *v_dest, void *v_src, int line);

//slicing, and cloning
void *getVectorSlice(void *v_vector, void *v_index);
void *getVectorCopy(void *v_vector);

//operations
void *dotProduct(void * v_vector_left, void * v_vector_right);
int   getIntDotProduct(void * v_vector_left, void * v_vector_right);
float getRealDotProduct(void * v_vector_left, void * v_vector_right);
