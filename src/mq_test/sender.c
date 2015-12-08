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
#include <stdio.h>   /* printf() sprintf() */
#include <mqueue.h>  /* mq_* functions */
#include <stdlib.h>  /* exit() */
#include <unistd.h>  /* getopt() */
#include <time.h>    /* ctime() and time() */
#include <string.h>  /* strlen() */
#include <errno.h>   /* errno perror() */
#include "test_params.h" /* NUM_OF_MSG, SIZE_OF_MSG */

/* max length of a message (just for this process) */
#define MQ_PRIO 10
#define BILLION 1E9
#define PERMS O_WRONLY | O_CREAT | O_EXCL
#define MODES S_IRWXU | S_IRWXG

/* Opens or creates a new message queue */
int open_message_queue(int, int);

int main(int argc, char *argv[]) {
    mqd_t msgq_id;
    char msgcontent[MAX_MSG_LEN] = {0};
    struct timespec requestStart, requestEnd;
    double accum;
    int i, j, k;
    int MSG_COUNT_PER_TEST = (int)sizeof(NUM_OF_MSG)/sizeof(int);
    int MSG_SIZE_PER_TEST = (int)sizeof(SIZE_OF_MSG)/sizeof(int);

    //printf("Wait 5 sec... \n");
    //sleep(5);

    for (k = 0; k < MSG_SIZE_PER_TEST; ++k){

    	memset(msgcontent, 'x', SIZE_OF_MSG[k]);
    	msgq_id = open_message_queue(SIZE_OF_MSG[k], k);
    	
    	mq_send(msgq_id, msgcontent, SIZE_OF_MSG[k], MQ_PRIO);
    	//mq_receive(msgq_id, msgcontent, SIZE_OF_MSG[k]+1, NULL);

	    for (j = 0; j < MSG_COUNT_PER_TEST; ++j)
	    {	
	    	printf("**********************************************************************\n");
	    	printf("Test %d is starting... [Number of messages:%d] [Size of a message:%d]\n", j+1, NUM_OF_MSG[j], SIZE_OF_MSG[k]);
	    	printf("msgq_id: %d\n", msgq_id);

	    	// Get initial time of test
		    clock_gettime(CLOCK_REALTIME, &requestStart);
		    
		    for ( i = 0; i < NUM_OF_MSG[j]; ++i){
		        mq_send(msgq_id, msgcontent, SIZE_OF_MSG[k], MQ_PRIO);
		    }
		    
		    // Get the ending time of test
		    clock_gettime(CLOCK_REALTIME, &requestEnd);

		    accum = ( requestEnd.tv_sec - requestStart.tv_sec ) 
		    + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / BILLION;

		    printf("Test %d is finished.\n", j+1);
		    printf( "Elapsed time: %.8lf sec. \n", accum );
		    printf("Avarage time: %.8lf sec. \n", accum/NUM_OF_MSG[j]);
		    printf("**********************************************************************\n\n\n");
		    sleep(1);
	    }
	
	    /* closing the queue */
	    mq_close(msgq_id);

	    /* remove the queue */
	    mq_unlink(MSGQOBJ_NAME);

	    sleep(1);
	}
    
    
    return 0;
}

int open_message_queue(int msgsize, int index){
		struct mq_attr msgq_attr = {0};
		mqd_t msgq_id;
		char MQNAME[1024] = {0};

		msgq_attr.mq_flags = 0;
		msgq_attr.mq_maxmsg = 10; //?
		msgq_attr.mq_msgsize = msgsize;
		msgq_attr.mq_curmsgs = 0;

		sprintf(MQNAME, "%s%d", MSGQOBJ_NAME, index);
		printf("mq_name : %s\n", MQNAME);

		mq_unlink(MQNAME);

		/* Create Queue */
		msgq_id = mq_open(MQNAME, PERMS, MODES, &msgq_attr);

		// If mq exist, try to open
		if( msgq_id == ((mqd_t) -1 ) && (errno == EEXIST) ){
			//msgq_id = mq_open(MQNAME, O_WRONLY, &msgq_attr);
		}

		if (msgq_id == (mqd_t)-1) {
		    perror("In mq_open()");
		    exit(1);
		}

		return msgq_id;
}