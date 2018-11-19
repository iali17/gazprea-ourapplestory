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

//helpers
size_t getMemberSize(int type);
void   setVectorVal(vector * v, int idx, void *val);
void  *getVectorElementPointer(void *v_vector, int idx);
void  *getNull(int type, void *ret);
void  *getIdentity(int type, void *ret);

//built ins
int   getVectorLength(void * v_vector);
void *getReverseVector(void * v_vector);
