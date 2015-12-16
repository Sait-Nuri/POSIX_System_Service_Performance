#include <stdio.h>   /* printf() sprintf() */
#include <sched.h>   /* sched_yield() */

int main(int argc, char *argv[]) {
    
    
    while(1){

    	/* Always relinquish the CPU */
    	sched_yield();
    }

    return 0;
}
