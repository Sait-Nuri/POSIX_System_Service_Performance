/*
 *  dropone.c
 *  
 *  drops a message into a #defined queue, creating it if user
 *  requested. The message is associated a priority still user
 *  defined
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
#define MAX_MSG_LEN     64
#define MQ_PRIO 10
#define BILLION 1E9
#define PERMS O_WRONLY | O_CREAT | O_EXCL

int main(int argc, char *argv[]) {
    mqd_t msgq_id;
    pid_t my_pid = getpid();
    char msgcontent[MAX_MSG_LEN] = {0};
    struct timespec requestStart, requestEnd;
    double accum;
    struct mq_attr msgq_attr = {0};
    int i;
    msgq_attr.mq_flags = 0;
    msgq_attr.mq_maxmsg = 10;
    msgq_attr.mq_msgsize = SIZE_OF_MSG[0];
    msgq_attr.mq_curmsgs = 0;

    mq_unlink(MSGQOBJ_NAME);

    /* Create Queue */
    msgq_id = mq_open(MSGQOBJ_NAME, PERMS, S_IRWXU | S_IRWXG, &msgq_attr);

    // If mq exist, try to open
    if( msgq_id == ((mqd_t) -1 ) && (errno == EEXIST) ){
    	
    	 /* opening the queue */
    	msgq_id = mq_open(MSGQOBJ_NAME, O_WRONLY, &msgq_attr);
    }

    if (msgq_id == (mqd_t)-1) {
        perror("In mq_open()");
        exit(1);
    }

    //snprintf(msgcontent, MAX_MSG_LEN, "Hello from process %u ", my_pid);

    printf("Wait 5 sec... ");
    sleep(5);

    mq_send(msgq_id, msgcontent, SIZE_OF_MSG[0], MQ_PRIO);

    // Get the starting time 
    clock_gettime(CLOCK_REALTIME, &requestStart);
    
    for ( i = 0; i < NUM_OF_MSG[0]; ++i){
        mq_send(msgq_id, msgcontent, SIZE_OF_MSG[0], MQ_PRIO);
    }
    
    // Get the ending time 
    clock_gettime(CLOCK_REALTIME, &requestEnd);

    accum = ( requestEnd.tv_sec - requestStart.tv_sec ) 
    + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / BILLION;

    printf( "Elapsed time: %.8lf\n", accum );
    printf("Avarage time: %.8lf\n", accum/NUM_OF_MSG[0]);

    /* closing the queue        -- mq_close() */
    mq_close(msgq_id);

    /*
    printf("Any key to finish... \n");
    getchar();
    */

    mq_unlink(MSGQOBJ_NAME);
    
    return 0;
}
