#include "../rt_headers/interval_tools.h"
#include "../rt_headers/vector_tools.h"

/**
 *
 * @param lower
 * @param upper
 * @return - new initialized vector
 */
void *getNewInterval(int lower, int upper){
    //allocate space
    interval *ret = (interval *) calloc(1, sizeof(interval));
    ret->lower    = (int *) malloc(sizeof(int));
    ret->upper    = (int *) malloc(sizeof(int));

    //set values
    *ret->lower = lower;
    *ret->upper = upper;

    return ret;
}

/**
 * convert interval to a vector
 * @param v_interval
 * @param by
 * @return - vector
 */
void *getVectorFromInterval(void * v_interval, int by){
    //get bounds
    int lower = *(((interval *) v_interval)->lower);
    int upper = *((interval *) v_interval)->upper;

    //get a vector
    int numElements =  (upper - lower + 1);
    vector *v = (vector *) getEmptyVector(INTEGER);
    initVector(v, numElements);

    //loop
    int i, j = 0;
    for(i = lower; i <= upper; i+=by)
        ((int *)v->elements)[j++] = i;

    //fix the return
    *v->numElements = j;

    return v;
}

/**
 *
 * @param v_interval
 */
void destroyInterval(void * v_interval){
    if(v_interval == NULL)
        return;

    interval * i = (interval *) v_interval;

    if(i->lower != NULL)
        free(i->lower);

    if(i->upper != NULL)
        free(i->upper);

    free(v_interval);
    v_interval = NULL;
}
