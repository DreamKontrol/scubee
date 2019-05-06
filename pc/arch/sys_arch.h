#ifndef ARCH_SYS_ARCH_H
#define ARCH_SYS_ARCH_H

#include <pthread.h>
#include <semaphore.h>

typedef pthread_mutex_t sys_mutex_t;

typedef struct {
    pthread_t thr;
    void ( *func )( void * );
    void *arg;
} thread_t, *sys_thread_t;

typedef sem_t sys_sem_t;

#define MBOX_SIZE 12
typedef struct {
    sys_sem_t sem;
    void *queue[ MBOX_SIZE ];
    int head;
    int tail;
} sys_mbox_t; // TODO

#endif
