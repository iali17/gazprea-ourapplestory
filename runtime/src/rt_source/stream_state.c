#include "../rt_headers/stream_state.h"
#include "../rt_headers/vector_tools.h"
#include <sys/ioctl.h>
#include <stropts.h>
#include <string.h>

int get_stream_state(void * v_stream){
    stream * s = (stream *) v_stream;
    return s->stream_state;
}
/*
 * Things to try when reading a character
 *  - ctrl-d should end the input immediately otherwise, they entered a character and we expect two ctrl-d inputs
 *  - put more than 3 characters into the buffer, the next read is skipped somehow. maybe the buffer isnt getting
 *      flushed properly?
 *  - this seems to read EOLN fine
 */
void read_in(void * v_stream, void * v_dest, int type) {
    //cast the void
    if (feof(stdin)) return;
    stream *s = (stream *) v_stream;

    //locals
    int ret;
    void *read = malloc(sizeof(int));
    FILE *inStream = fdopen(0, "r");

    //make a buffer
    char *inBuffer = (char *) calloc(3, sizeof(char *));
    memset(inBuffer, '\0', sizeof(inBuffer));

    //init read to null
    getNull(type, v_dest);

    //reading
    if (type == INTEGER){
        ret = fscanf(inStream, "%d", (int *) read); //ret now has the ascii value of the first character default '\0'
    }
    else if (type == CHAR){
        ret = *fgets(inBuffer, 3, inStream);
        *(char*) read = (char) ret;
    }
    else if(type == BOOLEAN) {
        ret = *fgets(inBuffer, 3, inStream);
        *(char *) read = (char) ret;
    }
    else if(type == REAL){
        ret = fscanf(inStream, "%f", (float *) read);
    }

    //debug string
    printf("\nretVal: %d, feofVal: %d (ret > 0 && feof) = %d buffStart: %d read: %c type: %d\n", ret, feof(inStream), (ret > 0 && feof(inStream)),inBuffer[0], *(char *) read, type);

    //boolean only garbage
    if (type == BOOLEAN && feof(inStream) && (*(char *)read == 'T' || *(char *)read == 'F' )) {
        s->stream_state = 0;
        if(*(char *)read == 'T') *(int *)read = 1;
        else *(int *)read = 0;
        assignValFromPointers(v_dest, read, INTEGER);
        fflush(inStream);
    } else if(type == BOOLEAN) {
        s->stream_state = 1;
        *(int *)read = 0;
        assignValFromPointers(v_dest, read, INTEGER);
        fflush(inStream);
    }

    //set the state
    if (ret > 0 && feof(inStream) && type != BOOLEAN){
        s->stream_state = 0;
        // write the value into the dest
        assignValFromPointers(v_dest, read, type);
        fflush(inStream);

    } else if (!feof(inStream)){
        s->stream_state = 1;
        fflush(inStream);
    } else if (ret <= 0 && feof(inStream)) {
        s->stream_state = 2;
        fflush(inStream);
    } else {
        s->stream_state = 2;
        fflush(inStream);
    }
    //fclose(inStream);
}