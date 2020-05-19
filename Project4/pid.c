/*
 ** Name: David Kopp
 ** Class: Comp322
 ** Project Name: Project4
 ** File Name: pid.c
 ** Date: 11-7-15
 ** Discription: This program shows how to use mutex locks and threads in C. It creates 100 threads,
 **              but can only 50 pids can be allocated at a give time. Mutex locks are used around the
 **              data structure to prevent a racing condition.
 */

#include "pid.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define PID_POOL 50

int* map; // 0 means avaliable / 1 means unavaliable

/**
 * Allocates the pid_map.
 */
int allocate_map(void)
{
    map = (int*)malloc(sizeof(int) * PID_POOL);
    
    if (map == NULL)
    {
        return -1;
    }
    int i;
    
    for (i = 0; i < PID_POOL; i++)
    {
        map[i] = 0;
    }
    
    return 1;
}

/**
 * Allocates a pid
 */
int allocate_pid(void)
{
    int i;
       
    for (i = 0; i < PID_POOL; i++)
    {
        if (map[i] == 0)
        {
            //printf("%d", map[i]);
            map[i] = 1;
            return i + PID_MIN;
        }
    }
    
    //printf("PID Assigned = %d\n", (i + PID_MIN));
    return -1;
}

/**
 * Releases a pid
 */
void release_pid(int pid)
{
    pthread_mutex_lock(&mutex);
    map[pid - PID_MIN] = 0;
    //printf("%d", pid - PID_MIN);
    pthread_mutex_unlock(&mutex);
}