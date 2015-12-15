#include <stdio.h>       /* printf() */
#include <stdlib.h>      /* malloc(), exit() */
#include <time.h>        /* clock_gettime() */
#include <errno.h>       /* errno, perror() */
#include <pthread.h>     /* pthread_create() */
#include <unistd.h>      /* sleep() */

#define MAX_THREAD 10
#define BILLION 1000000000
#define MILLION 1000000
#define THOUSAND 1000

void do_something(void *);

int main()
{
    int i;
    pthread_t threads[MAX_THREAD];
    struct timespec requestStart, requestEnd;
    unsigned long long result, accum = 0;

    for(i = 0; i < MAX_THREAD; ++i){

        printf("\r\n**********************************************************************\n\r");
        printf("Test %d is starting... \n\r", i+1);


        // Get initial time of test
        clock_gettime(CLOCK_REALTIME, &requestStart);

        if( pthread_create(&threads[i], NULL, (void *) &do_something, &i) != 0)
            perror("pthread_create"), exit(1);

        printf("Thread %d created.\n\r", i+1);

        // Get the ending time of test
        clock_gettime(CLOCK_REALTIME, &requestEnd);

        result = (( requestEnd.tv_sec - requestStart.tv_sec ) * BILLION)
            + ( requestEnd.tv_nsec - requestStart.tv_nsec ) ;

        accum += result;

        printf( "Pthread_create() elapsed time: %llu nanosec. \n\r", result );
        printf("**********************************************************************\n\r");
        sleep(1);
    }

    sleep(1);

    printf("All tests are finished.\n\r");
    printf("Avarage time: %llu nanosec. \n\r\n", accum/MAX_THREAD);

    return 0;
}

void do_something(void * arg){
    unsigned long long up_forever = 0;

    while(1){
        up_forever++;
    }

    printf("Thread %d almost end...", (int)pthread_self());
}

