#include "lwip/sys.h"

// Mutex

err_t sys_mutex_new( sys_mutex_t *mutex ) {
    pthread_mutex_init( mutex, 0 );
    return ERR_OK;
}

void sys_mutex_free( sys_mutex_t *mutex ) {
    pthread_mutex_destroy( mutex );
}

void sys_mutex_lock( sys_mutex_t *mutex ) {
    pthread_mutex_lock( mutex );
}

void sys_mutex_unlock( sys_mutex_t *mutex ) {
    pthread_mutex_unlock( mutex );
}

// Semaphore

err_t sys_sem_new( sys_sem_t *sem, uint8_t count ) {
    sem_init( sem, 0, 0 );
    return ERR_OK;
}

void sys_sem_free( sys_sem_t *sem ) {
    sem_close( sem );
}

uint32_t sys_arch_sem_wait( sys_sem_t *sem, uint32_t timeout ) {
    if ( timeout ) {
        struct timespec ts;
        clock_gettime( CLOCK_REALTIME, &ts );
        ts.tv_nsec += timeout * 1000000;
        if ( ts.tv_nsec >= 1000000000 ) {
            ts.tv_sec += ts.tv_nsec / 1000000000;
            ts.tv_nsec %= 1000000000;
        }
        if ( sem_timedwait( sem, &ts ) ) {
            return SYS_ARCH_TIMEOUT;
        }
    } else {
        sem_wait( sem );
    }
    return 0;
}

void sys_sem_signal( sys_sem_t *sem ) {
    sem_post( sem );
}

// Mailbox

err_t sys_mbox_new( sys_mbox_t *mbox, int size ) {
    sys_sem_new( &mbox->sem, 0 );
    for ( int i = 0; i < MBOX_SIZE; i++ ) {
        mbox->queue[ i ] = 0;
    }
    mbox->head = 0;
    mbox->tail = 0;
    return ERR_OK;
}

uint32_t sys_arch_mbox_fetch( sys_mbox_t *mbox, void **msg, uint32_t timeout ) {
    if ( mbox->head == mbox->tail ) {
        if ( timeout ) {
            timeout = sys_arch_sem_wait( &mbox->sem, timeout );
            if ( timeout ) {
                return timeout;
            }
        }
    }
    int tail = mbox->tail + 1;
    if ( msg ) {
        *msg = mbox->queue[ mbox->tail ];
    }
    if ( tail == MBOX_SIZE ) {
        tail = 0;
    }
    mbox->tail = tail;
    return 0;
}

err_t sys_mbox_trypost( sys_mbox_t *mbox, void *msg ) {
    int head = mbox->head + 1;
    mbox->queue[ mbox->head ] = msg;
    if ( head == MBOX_SIZE ) {
        head = 0;
    }
    mbox->head = head;
    sys_sem_signal( &mbox->sem );
    return ERR_OK;
}

void sys_mbox_post( sys_mbox_t *mbox, void *msg ) {
    sys_mbox_trypost( mbox, msg );
}

err_t sys_mbox_trypost_fromisr( sys_mbox_t *mbox, void *msg ) {
    return sys_mbox_trypost( mbox, msg );
}

int sys_mbox_valid( sys_mbox_t *mbox ) {
    return !!mbox;
}

// Thread

static void *threadWrapper( void *args ) {
    sys_thread_t thr = ( sys_thread_t )args;
    thr->func( thr->arg );
    return 0;
}

sys_thread_t sys_thread_new( const char *name, lwip_thread_fn func, void *arg, int stacksize, int prio ) {
    sys_thread_t thr = malloc( sizeof( thread_t ) );
    thr->func = func;
    thr->arg = arg;
    pthread_create( &thr->thr, 0, threadWrapper, thr->arg );
    return thr;
}
