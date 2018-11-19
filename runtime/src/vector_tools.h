#pragma once

#include "rt_globals.h"

vector *getVector(int type, int numElements);
void   *getEmptyVector(int type);
void   *getNull(int type, void *ret);
void   *getIdentity(int type, void *ret);
void   *getReverseVector(void * v_vector);
void   *getVectorElementPointer(void *v_vector, int idx);
void    setNullVector(void * v_vector);
void    setIdentityVector(void * v_vector);
void    setVectorVal(vector * v, int idx, void *val);
int     getVectorLength(void * v_vector);
size_t getMemberSize(int type);
void initVector(void * v_vector,  int numMem);