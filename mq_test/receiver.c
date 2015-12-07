/*
 *  takeone.c
 *  
 *  simply request a message from a queue, and displays queue
 *  attributes.
 *
 *
 *  Created by Mij <mij@bitchx.it> on 07/08/05.
 *  Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 *
 */

#include <stdio.h>  /* mq_* functions */
#include <mqueue.h> /* exit() */
#include <stdlib.h> /* getopt() */
#include <unistd.h> /* ctime() and time() */
#include <time.h>   /* strlen() */
#include <string.h> /* errno */
#include <errno.h>  /* perror() */

#include "test_params.h" /* NUM_OF_MSG, SIZE_OF_MSG */

/* name of the POSIX object referencing the queue */
#define MSGQOBJ_NAME    "/myqueue1"
/* max length of a message (just for this process) */
#define MAX_MSG_LEN 10000
#define BILLION 1E9
#define PERMS O_RDONLY

int main(int argc, char *argv[]) {
    int i;
    int msgsz;
    unsigned int sender;
    double accum;
    char msgcontent[MAX_MSG_LEN];
    struct mq_attr msgq_attr = {0};
    struct timespec requestStart, requestEnd;
    mqd_t msgq_id;
    
    /* opening the queue        --  mq_open() */
    msgq_id = mq_open(MSGQOBJ_NAME, PERMS);
    
    if (msgq_id == (mqd_t)-1) {
        perror("In mq_open()");
        exit(1);
    }

    /* getting the attributes from the queue        --  mq_getattr() */
    mq_getattr(msgq_id, &msgq_attr);
    printf("Queue \"%s\":\n\t- stores at most %ld messages\n\t- large at most %ld bytes each\n\t- currently holds %ld messages\n", MSGQOBJ_NAME, msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);

    msgsz = mq_receive(msgq_id, msgcontent, SIZE_OF_MSG[0]+1, &sender);

    // Get the starting time 
    clock_gettime(CLOCK_REALTIME, &requestStart);

    for (i = 0; i < NUM_OF_MSG[0]; ++i){
    	/* getting a message */
	    msgsz = mq_receive(msgq_id, msgcontent, SIZE_OF_MSG[0]+1, &sender);
	    
	    if (msgsz == -1) {
	        perror("In mq_receive()");
	        exit(1);
	    }	
    	//printf("Received message (%d bytes) from %d: %s\n", msgsz, sender, msgcontent);
    }

    // Get the ending time 
    clock_gettime(CLOCK_REALTIME, &requestEnd);

    accum = ( requestEnd.tv_sec - requestStart.tv_sec ) 
    + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / BILLION;

    printf( "Elapsed time: %.8lf\n", accum );
    printf("Avarage time: %.8lf\n", accum/NUM_OF_MSG[0]);

    /* closing the queue    --  mq_close() */
    mq_close(msgq_id);
    
    
    return 0;
}
