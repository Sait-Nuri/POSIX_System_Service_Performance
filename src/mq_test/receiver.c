#include <stdio.h>   /* printf() sprintf() */
#include <mqueue.h>  /* mq_* functions */
#include <stdlib.h>  /* exit() */
#include <unistd.h>  /* getopt() */
#include <time.h>    /* ctime() and time() */
#include <string.h>  /* strlen() */
#include <errno.h>   /* errno perror() */
#include "test_params.h" /* NUM_OF_MSG, SIZE_OF_MSG */

/* max length of a message (just for this process) */
#define BILLION 1E9
#define PERMS O_RDONLY

int open_message_queue();

int main(int argc, char *argv[]) {
    int i, j, k;
    double accum;
    mqd_t msgq_id;
    char msgcontent[MAX_MSG_LEN];
    struct timespec requestStart, requestEnd;
    int MSG_COUNT_PER_TEST = (int)sizeof(NUM_OF_MSG)/sizeof(int);
    int MSG_SIZE_PER_TEST = (int)sizeof(SIZE_OF_MSG)/sizeof(int);

    for (k = 0; k < MSG_SIZE_PER_TEST; ++k){

    	msgq_id = open_message_queue(k);

    	mq_receive(msgq_id, msgcontent, SIZE_OF_MSG[k]+1, NULL);
    	//mq_send(msgq_id, msgcontent, SIZE_OF_MSG[k], MQ_PRIO);

	    for (j = 0; j < MSG_COUNT_PER_TEST; ++j){
	    	printf("**********************************************************************\n");
	    	printf("Test %d is starting... [Number of messages:%d] [Size of a message:%d]\n", j+1, NUM_OF_MSG[j], SIZE_OF_MSG[k]);
	    	printf("msgq_id: %d\n", msgq_id);
		    clock_gettime(CLOCK_REALTIME, &requestStart);

		    for (i = 0; i < NUM_OF_MSG[j]; ++i){
			    mq_receive(msgq_id, msgcontent, SIZE_OF_MSG[k]+1, NULL);
		    }

		    // Get the ending time 
		    clock_gettime(CLOCK_REALTIME, &requestEnd);

		    accum = ( requestEnd.tv_sec - requestStart.tv_sec ) 
		    + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / BILLION;

		    printf("Test %d is finished.\n", j+1);
		    printf( "Elapsed time: %.8lf sec. \n", accum );
		    printf("Avarage time: %.8lf sec. \n", accum/NUM_OF_MSG[j]);
		    printf("**********************************************************************\n\n\n");
		    sleep(1);
		}

		/* closing the queue    --  mq_close() */
    	mq_close(msgq_id);

    	sleep(3);
	}
    
    return 0;
}
int open_message_queue(int index){
	mqd_t msgq_id;
	struct mq_attr msgq_attr = {0};
	char MQNAME[50] = {0};

	sprintf(MQNAME, "%s%d", MSGQOBJ_NAME, index);
	printf("mq_name : %s\n", MQNAME);

	/* opening the queue        --  mq_open() */
	msgq_id = mq_open(MQNAME, PERMS);

	if (msgq_id == (mqd_t)-1) {
	    perror("In mq_open()");
	    exit(1);
	}

	mq_getattr(msgq_id, &msgq_attr);
	printf("Queue \"%s\":\n\t- stores at most %ld messages\n\t- large at most %ld bytes each\n\t- currently holds %ld messages\n", MQNAME, msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);

	return msgq_id;
}