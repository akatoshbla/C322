/*
 ** Name: David Kopp
 ** Class: Comp322
 ** Project Name: Project4
 ** File Name: pid.h
 ** Date: 11-7-15
 ** Discription: This program shows how to use mutex locks and threads in C. It creates 100 threads,
 **              but can only 50 pids can be allocated at a give time. Mutex locks are used around the
 **              data structure to prevent a racing condition.
 */

// File 1, pid.h
// This header file is included in pid.c and djk93533.c

#include <pthread.h>

#define PID_MIN  	300
#define PID_MAX 	350

/* mutex lock for accessing pid_map */
pthread_mutex_t mutex;

int pid_map[PID_MAX+1];

int last;	// last pid in use

void release_pid(int pid);
int allocate_pid(void);
int allocate_map(void);