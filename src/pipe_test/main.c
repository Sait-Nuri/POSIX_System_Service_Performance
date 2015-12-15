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
    unsigned long long accum = 0, result;
    int i, j, k;
    int MSG_COUNT_PER_TEST = (int)sizeof(NUM_OF_MSG)/sizeof(int);
    int MSG_SIZE_PER_TEST = (int)sizeof(SIZE_OF_MSG)/sizeof(int);

    for (k = 0; k < MSG_SIZE_PER_TEST; ++k){

        /* Fill the array with 'x' character */
        memset(buf, 'x', SIZE_OF_MSG[k]);

        /* Write buffer to named pipe */
        write(fd, buf, SIZE_OF_MSG[k]);

        for (j = 0; j < MSG_COUNT_PER_TEST; ++j)
        {
            printf("**********************************************************************\n\r");
            printf("Writer Test %d is starting... [Number of messages:%d] [Size of a message:%d]\n\r", j+1, NUM_OF_MSG[j], SIZE_OF_MSG[k]);
            //printf("File descriptor: %d\n\r", fd);

            // Get initial time of test
            clock_gettime(CLOCK_REALTIME, &requestStart);

            for ( i = 0; i < NUM_OF_MSG[j]; ++i){
                write(fd, buf, SIZE_OF_MSG[k]);
            }

            // Get the ending time of test
            clock_gettime(CLOCK_REALTIME, &requestEnd);

            result = ( requestEnd.tv_sec - requestStart.tv_sec )
            + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / BILLION;

            accum += result;

            printf("Writer Test %d is finished.\n\r", j+1);
            printf("Writer Elapsed time: %llu nanosec. \n\r", result );
            sleep(1);
        }

        printf("Writer Avarage time: %llu nanosec. \n\r", accum/NUM_OF_MSG[j]);
        printf("**********************************************************************\n\r\n\r\n\r");
        accum = 0;
        sleep(1);
    }

    /* close the FIFO */
    close(fd);

    return 0;
}

int reader(int fd){
    char buf[MAX_BUF];
    struct timespec requestStart, requestEnd;
    unsigned long long accum = 0, result;
    int i, j, k;
    int MSG_COUNT_PER_TEST = (int)sizeof(NUM_OF_MSG)/sizeof(int);
    int MSG_SIZE_PER_TEST = (int)sizeof(SIZE_OF_MSG)/sizeof(int);


    for (k = 0; k < MSG_SIZE_PER_TEST; ++k){

        /* Read incoming data with buffer */
        read(fd, buf, SIZE_OF_MSG[k]);

        for (j = 0; j < MSG_COUNT_PER_TEST; ++j){

            printf("**********************************************************************\n\r");
            printf("Reader Test %d is starting... [Number of messages:%d] [Size of a message:%d]\n\r", j+1, NUM_OF_MSG[j], SIZE_OF_MSG[k]);
            //printf("File descriptor: %d\n\r", fd);

            // Get initial time of test
            clock_gettime(CLOCK_REALTIME, &requestStart);

            for ( i = 0; i < NUM_OF_MSG[j]; ++i){
                read(fd, buf, SIZE_OF_MSG[k]);
            }

            // Get the ending time of test
            clock_gettime(CLOCK_REALTIME, &requestEnd);

            result = ( requestEnd.tv_sec - requestStart.tv_sec )
            + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / BILLION;

            accum += result;

            printf("Reader Test %d is finished.\n\r", j+1);
            printf("Reader Elapsed time: %llu nanosec. \n\r", result );

            sleep(1);
        }

        printf("Reader Avarage time: %llu nanosec. \n\r", accum/NUM_OF_MSG[j]);
        printf("**********************************************************************\n\r\n\r\n\r");
        sleep(1);
    }

    /* close the FIFO */
    close(fd);

    return 0;
}
