/*
	WARNING: In this test, underlying system is considered has only one core, 
	otherwise it won't give realistic result! . 

*/

#include <stdio.h>   	  /* printf() sprintf() */
#include <sched.h>   	  /* sched_yield() */
#include <time.h>         /* clock_getstime() */

#define BILLION 1000000000
#define MILLION 1000000
#define THOUSAND 1000

#define N_LOOP 20
#define COUNT 1000

unsigned long long measure_single_loop_time();
unsigned long long measure_task_switching_time();
void compute_task_switching_time(unsigned long long, unsigned long long);

int main(int argc, char *argv[]) {
	unsigned long long one_cycle_time, task_switch_time;

    one_cycle_time = measure_single_loop_time();    

    task_switch_time = measure_task_switching_time();

    compute_task_switching_time(one_cycle_time, task_switch_time);

    return 0;
}

unsigned long long measure_single_loop_time(){
	struct timespec start1, end1;
    unsigned long long loop_time, loop_total_time;
    int n = N_LOOP;
    int i;

    for ( i = 0; i < COUNT; ++i)
    {
    	// Get initial time of test
	    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start1);

	    while(--n){

	    }

	    // Get the ending time of test
	    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end1);

	    loop_time = (( end1.tv_sec - start1.tv_sec ) * BILLION)
            + ( end1.tv_nsec - start1.tv_nsec ) ;
	    
	    printf("Elapsed time for during n-loop: %llu nanosec. \n\r", loop_time );

	    loop_total_time += loop_time;
	    n = N_LOOP;
    }
    
    printf("Avarage time of nloop: %llu nanosec. \n\r", loop_total_time / COUNT );
    printf("Avarage time of one cycle: %d nanosec. \n\r", (int)(loop_total_time / COUNT)/ N_LOOP);

    return (loop_total_time / COUNT) / N_LOOP;
}

/* In this part task switching latency that is caused by calling sched_yield() from this and 
 * another task mutually, is measured   
 * 
 * Return value involves ( both caller and another process's one loop cycle + Task switching time )
 */
unsigned long long measure_task_switching_time(){
	struct timespec start1, end1;
    unsigned long long loop_time;
    int N = N_LOOP;

    // Get initial time of test
    clock_gettime(CLOCK_REALTIME, &start1);

    while(--N){

    	/* relinquish the CPU */
    	sched_yield();
	}

	// Get the ending time of test
    clock_gettime(CLOCK_REALTIME, &end1);

    loop_time = (( end1.tv_sec - start1.tv_sec ) * BILLION)
        + ( end1.tv_nsec - start1.tv_nsec ) ;
	
	printf("Elapsed time for during n-loop sched_yield(): %llu nanosec. \n\r", loop_time );

	return loop_time / N_LOOP;
}

/*
*  This function computes task switching time by subtracting unnecessary latencies during measurement.
*  task_switch_time: Involves two times Task switching latency and one cycle time of loop (2 * (one loop cycle + Task switching))
*  one_cycle_time: Involves one cycle time of loop
*/
void compute_task_switching_time(unsigned long long one_cycle_time, unsigned long long task_switch_time){
	unsigned long long tsl;

	tsl = (task_switch_time / 2) - one_cycle_time;

	printf("Task switching latency: %llu nanosec \n", tsl);
}