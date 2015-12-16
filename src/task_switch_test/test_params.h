#ifndef _TESTPARAMS_H
#define _TESTPARAMS_H

/* MAX_MSG_LEN must be greater than the biggest element of SIZE_OF_MSG[] array */
#define MAX_MSG_LEN     2048

/* name of the POSIX object referencing the queue */
#define MSGQOBJ_NAME    "/myqueue"

const int NUM_OF_MSG[] = { 10, 100, 1000, 10000, 100000};
const int SIZE_OF_MSG[] = {1, 16, 64,  256, 1024};

#endif