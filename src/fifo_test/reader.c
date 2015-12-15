#include <stdio.h>       /* printf() sprintf() */
#include <stdlib.h>      /* exit() */
#include <unistd.h>      /* getopt() */
#include <sys/types.h>   /* mkfifor() */
#include <time.h>        /* ctime() and time() */
#include <string.h>      /* strlen() */
#include <errno.h>       /* errno perror() */
#include <sys/stat.h>    /* S_IRWXU | S_IRWXG */
#include <fcntl.h>       /* PERMS */
#include "test_params.h" /* MAX_BUF NUM_OF_MSG, SIZE_OF_MSG */

#define PERMS O_WRONLY

int open_fifo(int);

int main()
{
    int fd;
    char buf[MAX_BUF];
    struct timespec requestStart, requestEnd;
    unsigned long long accum = 0, result;
    int i, j, k;
    int MSG_COUNT_PER_TEST = (int)sizeof(NUM_OF_MSG)/sizeof(int);
    int MSG_SIZE_PER_TEST = (int)sizeof(SIZE_OF_MSG)/sizeof(int);

    /* create the FIFO (named pipe) */
    fd = open_fifo(0);

    for (k = 0; k < MSG_SIZE_PER_TEST; ++k){

        /* Read incoming data with buffer */
        read(fd, buf, SIZE_OF_MSG[k]+1);

        for (j = 0; j < MSG_COUNT_PER_TEST; ++j){

        	printf("**********************************************************************\n");
	    	printf("Test %d is starting... [Number of messages:%d] [Size of a message:%d]\n", j+1, NUM_OF_MSG[j], SIZE_OF_MSG[k]);
	    	printf("File descriptor: %d\n", fd);

	    	// Get initial time of test
            clock_gettime(CLOCK_REALTIME, &requestStart);

            for ( i = 0; i < NUM_OF_MSG[j]; ++i){
                read(fd, buf, MAX_BUF);
            }

            // Get the ending time of test
            clock_gettime(CLOCK_REALTIME, &requestEnd);

            result = ( requestEnd.tv_sec - requestStart.tv_sec ) 
            + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / BILLION;

            printf("Test %d is finished.\n", j+1);
            printf( "Elapsed time: %llu nanosec. \n", result );
            
            sleep(1);
        }

        printf("Avarage time: %llu nanosec. \n", accum/NUM_OF_MSG[j]);
        printf("**********************************************************************\n\n\n");

        accum = 0;
        sleep(2);
    }

    /* close the FIFO */
    close(fd);

    return 0;
}

int open_fifo(int x){
    int fd;
    char * myfifo = PIPE_PATH;

    fd = open(myfifo, O_RDONLY);

    if (fd == -1) {
        perror("In open_fifo()");
        exit(1);
    }

    printf("Fifo opened: %d \n", fd);

    return fd;
}