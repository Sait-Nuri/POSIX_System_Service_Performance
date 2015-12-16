#include <stdio.h>       /* printf() */
#include <stdlib.h>      /* malloc(), exit() */
#include <time.h>        /* clock_getstime() */
#include <errno.h>       /* errno, perror() */
#include <unistd.h>      /* sleep() */
#include <sys/mman.h>    /* shm_* stuff, and mmap() */
#include <fcntl.h>       /* O_CREAT | O_EXCL | O_RDWR */
#include <sys/stat.h>    /* S_IRWXU | S_IRWXG */
#include <string.h>      /* memset() */
#include "test_params.h" /* SH_MEM_PATH, SIZE_OF_MEM[] */

#define PERMS S_IRWXU | S_IRWXG
#define MODES O_CREAT | O_EXCL | O_RDWR
#define PROTECTION PROT_READ | PROT_WRITE
#define BILLION 1000000000
#define MILLION 1000000
#define THOUSAND 1000

int main(int argc, char const *argv[])
{
    int i;
    int fd;
    char* sh_mem;
    struct timespec start1, start2, end1, end2;
    unsigned long long result1, result2, accum1 = 0, accum2 = 0;
    int MEM_SIZE_PER_TEST = (int)sizeof(SIZE_OF_MEM)/sizeof(int);

    for(i = 0; i < MEM_SIZE_PER_TEST; ++i){

        printf("\r\n**********************************************************************\n\r");
        printf("Test %d is starting... [Memory size: %d]\n\r", i+1, SIZE_OF_MEM[i]);

        // Get initial time of test
        clock_gettime(CLOCK_REALTIME, &start1);

        // Open a shared memory
        fd = shm_open(SH_MEM_PATH, MODES, PERMS);
        if (fd < 0)
            perror("In shm_open()"), exit(1);

        /* adjusting mapped file size (make room for the whole segment to map)      --  ftruncate() */
        if( ftruncate(fd, SIZE_OF_MEM[i]) != 0 )
            perror("In ftruncate()"), exit(1);

        /* requesting the shared segment    --  mmap() */    
        sh_mem = (char *) mmap(NULL, SIZE_OF_MEM[i], PROTECTION, MAP_SHARED, fd, 0);
        if (sh_mem == NULL) 
            perror("In mmap()"), exit(1);

        // Get the ending time of test
        clock_gettime(CLOCK_REALTIME, &end1);

        result1 = (( end1.tv_sec - start1.tv_sec ) * BILLION)
            + ( end1.tv_nsec - start1.tv_nsec ) ;

        accum1 += result1;

        printf("Shared memory allocation elapsed time: %llu nanosec. \n\r", result1 );
        
        memset(sh_mem, 0, SIZE_OF_MEM[i]);

        sh_mem[0] = 1;

        // Wait for that client open the shared memory
        while(sh_mem[0]){}        

        // Get initial time of test
        clock_gettime(CLOCK_REALTIME, &start2);

        if( munmap(sh_mem, SIZE_OF_MEM[i]) != 0 )
            perror("In mmap()"), exit(1);

        if (shm_unlink(SH_MEM_PATH) != 0)
            perror("In shm_unlink()"), exit(1);

        // Get the ending time of test
        clock_gettime(CLOCK_REALTIME, &end2);

        result2 = (( end2.tv_sec - start2.tv_sec ) * BILLION)
            + ( end2.tv_nsec - start2.tv_nsec ) ;

        accum2 += result2;

        printf("Shared memory de-allocation elapsed time: %llu nanosec. \n\r", result2 );
        printf("**********************************************************************\n\r");

        sleep(1);
    }

    sleep(1);

    printf("\nAll tests are finished.\n\r");
    printf("Avarage time for allocation: %llu nanosec. \n\r", accum1/MEM_SIZE_PER_TEST);
    printf("Avarage time for de-allocation: %llu nanosec. \n\r\n", accum2/MEM_SIZE_PER_TEST);

    return 0;
}
