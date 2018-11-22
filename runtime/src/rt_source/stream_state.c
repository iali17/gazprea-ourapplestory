#include "../rt_headers/stream_state.h"
#include "../rt_headers/vector_tools.h"
#include <sys/ioctl.h>
#include <stropts.h>

int get_stream_state(void * v_stream){
    stream * s = (stream *) v_stream;
    return s->stream_state;
}

void read_in(void * v_stream, void * v_dest, int type){
    stream * s = (stream *) v_stream;
    int ret;
    void * read = malloc(sizeof(int));
    getNull(type, v_dest);

    //reading
    if(type == INTEGER)
        ret = scanf("%d", (int *) read);
    else if(type == CHAR)
        ret = scanf("%c", (char *) read);
    else if(type == BOOLEAN)
        ret = scanf("%c", (char *) read);
    else if(type == REAL)
        ret = scanf("%f", (float *) read);


    //set the state
    //printf("retVal: %d, feofVal: %d read: %c type: %d\n", ret, feof(stdin), *(char *) read, type);
    if (ret == 1){
        s->stream_state = 0;
        // write the value into the dest
        assignValFromPointers(v_dest, read, type);
    } else if (!feof(stdin)){
        s->stream_state = 1;
    } else {
        s->stream_state = 2;
    }
/*
    char c;
    while((c =fgetc(stdin)) != EOF) {
        printf("in buffer: %c\n", c);
    }*/
/*
    int k = 0;
    while(scanf("%*c")){
        printf("%d\n", k);
    }*/

}