#include "mystd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_SIZE 128 

static char *buf1 = NULL, *buf2 = NULL; /* lines without, with tabs expanded */
static int buf_size = START_SIZE;           /* size of both buffers if non-NULL */

void checkptr(void *p){
	if(p == NULL) printf("out of memory\n");
	}

void new_buffers(void) {
    checkptr(buf1 = (char *) realloc(buf1, buf_size));
    checkptr(buf2 = (char *) realloc(buf2, buf_size));
}

char *mygetline (FILE *fp) {

if (buf1==NULL) {
    checkptr(buf1 = (char *) malloc (buf_size));
    checkptr(buf2 = (char *) malloc (buf_size));
}
    buf1=fgets(buf1, buf_size, fp);
    if (buf1==NULL) return buf1; /* end of file */
    while (buf1[strlen(buf1)-1] != '\n') { /* failed to get whole line */
        buf_size *= 2;
        new_buffers();
        if (fgets(buf1+strlen(buf1),buf_size-strlen(buf1),fp)==NULL)
            return buf1; /* end of file */
    }
    return buf1;
}

