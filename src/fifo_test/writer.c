#include <stdio.h>       /* printf() sprintf() */
#include <stdlib.h>      /* exit() */
#include <unistd.h>      /* getopt() */
#include <sys/types.h>   /* mkfifor() */
#include <time.h>        /* ctime() and time() */
#include <string.h>      /* strlen() */
#include <errno.h>       /* errno perror() */
#include <sys/stat.h>    /* S_IRWXU | S_IRWXG */
#include <fcntl.h>       /* PERMS */
#include "test_params.h" /* NUM_OF_MSG, SIZE_OF_MSG */

#define PERMS O_WRONLY
#define MODES S_IRWXU | S_IRWXG

int open_fifo(int);


int main()
{
    int fd;
    char buf[MAX_BUF];
    struct timespec requestStart, requestEnd;
    double accum;
    int i, j, k;
    int MSG_COUNT_PER_TEST = (int)sizeof(NUM_OF_MSG)/sizeof(int);
    int MSG_SIZE_PER_TEST = (int)sizeof(SIZE_OF_MSG)/sizeof(int);

    /* create the FIFO (named pipe) */
    fd = open_fifo(0);


    for (k = 0; k < MSG_SIZE_PER_TEST; ++k){

        /* Fill the array with 'x' character */
        memset(buf, 'x', SIZE_OF_MSG[k]);

        /* Write buffer to named pipe */
        write(fd, buf, MAX_BUF);
        
        for (j = 0; j < MSG_COUNT_PER_TEST; ++j)
        {     
            printf("**********************************************************************\n");
            printf("Test %d is starting... [Number of messages:%d] [Size of a message:%d]\n", j+1, NUM_OF_MSG[j], SIZE_OF_MSG[k]);
            printf("File descriptor: %d\n", fd);

            // Get initial time of test
            clock_gettime(CLOCK_REALTIME, &requestStart);

            for ( i = 0; i < NUM_OF_MSG[j]; ++i){
                write(fd, buf, MAX_BUF);
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
        
         sleep(2);
    }

    /* close the FIFO */
    close(fd);

    /* remove the FIFO */
    unlink(PIPE_PATH);

    return 0;
}

int open_fifo(int x){
    int fd;
     char * myfifo = PIPE_PATH;

    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);

    /* write "Hi" to the FIFO */
    fd = open(myfifo, O_WRONLY);

    if (fd == -1) {
        perror("In open_fifo()");
        exit(1);
    }

    printf("Fifo opened: %d \n", fd);

    return fd;
}