#include "../rt_headers/stream_state.h"

int get_stream_state(void * v_stream){
    stream * s = (stream *) v_stream;
    return s->stream_state;
}

void read_in(void * v_stream, void * v_dest, int type){
    stream * s = (stream *) v_stream;
    int ret;

    //reading
    if(type == INTEGER)
        ret = scanf("%d", (int *) v_dest);
    else if(type == CHAR)
        ret = scanf("%c", (char *) v_dest);
    else if(type == BOOLEAN)
        ret = scanf("%c", (char *) v_dest);
    else if(type == REAL)
        ret = scanf("%f", (float *) v_dest);

    //set the state
    s->stream_state = ret && (feof(stdin));
}