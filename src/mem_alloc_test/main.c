#include <stdio.h>       /* printf() */
#include <stdlib.h>      /* malloc(), exit() */
#include <time.h>        /* clock_gettime() */
#include <errno.h>       /* errno, perror() */
#include <unistd.h>      /* sleep() */

const int SIZE_OF_MEM[] = {1, 16, 64,  256, 1024, 4096, 8192};
const int BILLION = 1000000000;

int main()
{
    void* ptr;
    int i;
    unsigned long long result1, result2, sum1 = 0, sum2 = 0;
    struct timespec start1, start2, end1, end2;
    int NUM_OF_TEST = (int)sizeof(SIZE_OF_MEM)/sizeof(int);

    for (i = 0; i < NUM_OF_TEST; ++i){
        printf("**********************************************************************\n\r");
        printf("Test %d is starting... [memory allocation size: %d]\n\r", i+1, SIZE_OF_MEM[i] );

        // Get initial time of test
        clock_gettime(CLOCK_REALTIME, &start1);

        ptr = (void*)malloc( SIZE_OF_MEM[i] );

        // Get the ending time of test
        clock_gettime(CLOCK_REALTIME, &end1);

        if(ptr == NULL){
            perror("In malloc()");
            exit(-1);
        }

        // Get initial time of test
        clock_gettime(CLOCK_REALTIME, &start2);

        /* Free memory */
        free(ptr);

        // Get the ending time of test
        clock_gettime(CLOCK_REALTIME, &end2);

        result1 = ( ( end1.tv_sec - start1.tv_sec ) * BILLION)
            + ( end1.tv_nsec - start1.tv_nsec );
        
        sum1 += result1;
        
        result2 = ( ( end2.tv_sec - start2.tv_sec ) * BILLION)
            + ( end2.tv_nsec - start2.tv_nsec );

        sum2 += result2;

        printf("Test %d is finished.\n\r", i+1);

        printf( "Elapsed time for malloc() : %llu nanosec. \n\r", result1 );
        printf( "Elapsed time for free() : %llu nanosec. \n\r", result2 );
        printf("**********************************************************************\n\r");

        sleep(1);
    }

    printf("All tests are finished.\n\r");
    printf("Avarage time for malloc(): %llu nanosec. \n\r", sum1/NUM_OF_TEST);
    printf("Avarage time for free(): %llu nanosec. \n\r", sum2/NUM_OF_TEST);

    return 0;
}

