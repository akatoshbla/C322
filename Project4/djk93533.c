/*
 ** Name: David Kopp
 ** Class: Comp322
 ** Project Name: Project4
 ** File Name: djk93533.c
 ** Date: 11-7-15
 ** Discription: This program shows how to use mutex locks and threads in C. It creates 100 threads, 
 **              but can only 50 pids can be allocated at a give time. Mutex locks are used around the
 **              data structure to prevent a racing condition.
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pid.h"

#define NUM_THREADS 100
#define ITERATIONS	10
#define SLEEP		5

/**
 * mutex lock used when accessing data structure
 * to ensure there are no duplicate pid's in use.
 */
pthread_mutex_t test_mutex;
int counter = 1;

// allocate the pid to the thread [*allocator]
void *allocator(void *param)
{
    int last_pid;
    int threadNumber;
    
    pthread_mutex_lock(&test_mutex);
    
    last_pid = allocate_pid();
    while (last_pid == -1)
    {
        last_pid = allocate_pid();
    }
    threadNumber = counter;
    printf("Allocated Thread PID: %d ------- Thread#%d\n", last_pid, counter);
    counter++;
    pthread_mutex_unlock(&test_mutex);
    
    // Do something here Crit section
    
    int r = rand() % SLEEP + 1;
    sleep(r);
    
    
    
    release_pid(last_pid);
    printf("Released Thread PID: %d ------- Thread#%d\n", last_pid, threadNumber);
    
    
    return NULL;
}

int main(void)
{
	int i;
    int error;
	pthread_t tids[NUM_THREADS];

	//Todo: init mutex here
    error = pthread_mutex_init(&mutex, NULL) + pthread_mutex_init(&test_mutex, NULL);
    
    
    if (error != 0)
    {
        printf("Mutex failed to Initialize!\n");
        return 1;
    }
    
	// allocate the pid map
	if (allocate_map() == -1)
    {
		return -1;
    }
	
	// Todo: Make all the threads
    for (i = 0; i < NUM_THREADS; i++)
    {
        error = pthread_create(&(tids[i]), NULL, &allocator, NULL);
        
        if (error != 0)
        {
            printf("Thread failed to be created!\n");
            return 1;
        }
    }
	
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tids[i], NULL);
    }
    
	printf("***DONE***\n");

	return 0;
}