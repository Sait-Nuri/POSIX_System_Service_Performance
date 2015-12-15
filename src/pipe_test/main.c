#include <stdio.h>       /* printf() sprintf() */
#include <stdlib.h>      /* exit() */
#include <unistd.h>      /* getopt() */
#include <time.h>        /* ctime() and time() */
#include <string.h>      /* strlen() */
#include <errno.h>       /* errno perror() */

#include "test_params.h" /* NUM_OF_MSG, SIZE_OF_MSG */

#define PERMS O_WRONLY
#define MODES S_IRWXU | S_IRWXG

int writer(int);
int reader(int);

int main()
{
    int  fd[2], nbytes;
    pid_t   childpid;

    pipe(fd);

    if((childpid = fork()) == -1)
    {
            perror("fork");
            exit(1);
    }

	if(childpid == 0)
    {
            /* Child process closes up input side of pipe */
            close(fd[0]);

            /* Send file descriptor through the output side of pipe */
            writer(fd[1]);

            exit(0);
    }
    else
    {
            /* Parent process closes up output side of pipe */
            close(fd[1]);

            /* Read in a string from the pipe */
            nbytes = reader(fd[0]);
    }

    return 0;
}


int writer(int fd){

	char buf[MAX_BUF];
    struct timespec requestStart, requestEnd;
    double accum;
    int i, j, k;
    int MSG_COUNT_PER_TEST = (int)sizeof(NUM_OF_MSG)/sizeof(int);
    int MSG_SIZE_PER_TEST = (int)sizeof(SIZE_OF_MSG)/sizeof(int);

    for (k = 0; k < MSG_SIZE_PER_TEST; ++k){

        /* Fill the array with 'x' character */
        memset(buf, 'x', SIZE_OF_MSG[k]);

        /* Write buffer to named pipe */
        write(fd, buf, MAX_BUF);
        
        for (j = 0; j < MSG_COUNT_PER_TEST; ++j)
        {     
            printf("**********************************************************************\n");
            printf("Writer Test %d is starting... [Number of messages:%d] [Size of a message:%d]\n", j+1, NUM_OF_MSG[j], SIZE_OF_MSG[k]);
            //printf("File descriptor: %d\n", fd);

            // Get initial time of test
            clock_gettime(CLOCK_REALTIME, &requestStart);

            for ( i = 0; i < NUM_OF_MSG[j]; ++i){
                write(fd, buf, MAX_BUF);
            }

            // Get the ending time of test
            clock_gettime(CLOCK_REALTIME, &requestEnd);

            accum = ( requestEnd.tv_sec - requestStart.tv_sec ) 
            + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / BILLION;

            printf("Writer Test %d is finished.\n", j+1);
            printf("Writer Elapsed time: %.8lf sec. \n", accum );
            printf("Writer Avarage time: %.8lf sec. \n", accum/NUM_OF_MSG[j]);
            printf("**********************************************************************\n\n\n");
            sleep(1);
        }
        
         sleep(2);
    }

    /* close the FIFO */
    close(fd);

    return 0;
}

int reader(int fd){
	char buf[MAX_BUF];
    struct timespec requestStart, requestEnd;
    double accum;
    int i, j, k;
    int MSG_COUNT_PER_TEST = (int)sizeof(NUM_OF_MSG)/sizeof(int);
    int MSG_SIZE_PER_TEST = (int)sizeof(SIZE_OF_MSG)/sizeof(int);


    for (k = 0; k < MSG_SIZE_PER_TEST; ++k){

        /* Read incoming data with buffer */
        read(fd, buf, SIZE_OF_MSG[k]+1);

        for (j = 0; j < MSG_COUNT_PER_TEST; ++j){

        	printf("**********************************************************************\n");
	    	printf("Reader Test %d is starting... [Number of messages:%d] [Size of a message:%d]\n", j+1, NUM_OF_MSG[j], SIZE_OF_MSG[k]);
	    	//printf("File descriptor: %d\n", fd);

	    	// Get initial time of test
            clock_gettime(CLOCK_REALTIME, &requestStart);

            for ( i = 0; i < NUM_OF_MSG[j]; ++i){
                read(fd, buf, MAX_BUF);
            }

            // Get the ending time of test
            clock_gettime(CLOCK_REALTIME, &requestEnd);

            accum = ( requestEnd.tv_sec - requestStart.tv_sec ) 
            + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / BILLION;

            printf("Reader Test %d is finished.\n", j+1);
            printf("Reader Elapsed time: %.8lf sec. \n", accum );
            printf("Reader Avarage time: %.8lf sec. \n", accum/NUM_OF_MSG[j]);
            printf("**********************************************************************\n\n\n");
            sleep(1);
        }

        sleep(2);
    }

    /* close the FIFO */
    close(fd);

    return 0;
}