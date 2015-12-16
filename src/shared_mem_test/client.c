#include <stdio.h>       /* printf() */
#include <stdlib.h>      /* malloc(), exit() */
#include <time.h>        /* clock_getstime() */
#include <errno.h>       /* errno, perror() */
#include <unistd.h>      /* sleep() */
#include <string.h>      /* memset() */
#include <sys/mman.h>    /* shm_* stuff, and mmap() */
#include <fcntl.h>       /* O_CREAT | O_EXCL | O_RDWR */
#include <sys/stat.h>    /* S_IRWXU | S_IRWXG */
#include "test_params.h" /* SH_MEM_PATH, SIZE_OF_MEM[] */

#define PERMS S_IRWXU | S_IRWXG
#define MODES O_RDWR
#define PROTECTION PROT_READ | PROT_WRITE
#define BILLION 1000000000
#define MILLION 1000000
#define THOUSAND 1000

int main(int argc, char const *argv[])
{
	char* sh_mem;
    struct timespec start, end;
    unsigned long long result, accum = 0;
    int MEM_SIZE_PER_TEST = (int)sizeof(SIZE_OF_MEM)/sizeof(int);
    int fd, i;

    for (i = 0; i < MEM_SIZE_PER_TEST; ++i)
    {
    	// Get initial time of test
        clock_gettime(CLOCK_REALTIME, &start);

        // Open a shared memory
        fd = shm_open(SH_MEM_PATH, MODES, PERMS);
        if (fd < 0)
            perror("In shm_open()"), exit(1);

        /* requesting the shared segment */    
        sh_mem = (char *) mmap(NULL, SIZE_OF_MEM[i], PROTECTION, MAP_SHARED, fd, 0);
        if (sh_mem == NULL) 
            perror("In mmap()"), exit(1);

        // Get the ending time of test
        clock_gettime(CLOCK_REALTIME, &end);

        result = (( end.tv_sec - start.tv_sec ) * BILLION)
            + ( end.tv_nsec - start.tv_nsec ) ;

        accum += result;

        while(sh_mem[0] != 1){}

        memset(sh_mem, 0, SIZE_OF_MEM[i]);

    	sleep(2);
    }

    printf("Avarage time for open: %llu nanosec. \n\r", accum/MEM_SIZE_PER_TEST);

	return 0;
}