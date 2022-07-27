#ifndef CCDS_LOCK_H
#define CCDS_LOCK_H

#include <pthread.h>
#include <sys/types.h>


typedef pthread_cond_t   ccds_condv;
#define ccds_condv_init(condv)          pthread_cond_init(condv, NULL)
#define ccds_condv_destroy(condv)       pthread_cond_destroy(condv)

#define ccds_condv_wait(condv, lock)    pthread_cond_wait(condv, lock)
#define ccds_condv_signal(condv)        pthread_cond_signal(condv)


/* Threading locks wrapper functions, allows users to change the locking objects */
typedef pthread_rwlock_t ccds_rwlock;

#define ccds_rwlock_init(rwlock)        pthread_rwlock_init(rwlock, NULL)
#define ccds_rwlock_destroy(rwlock)     pthread_rwlock_destroy(rwlock)

#define ccds_rwlock_rlock(rwlock)       pthread_rwlock_rdlock(rwlock)
#define ccds_rwlock_wlock(rwlock)       pthread_rwlock_wrlock(rwlock)

#define ccds_rwlock_runlock(rwlock)     pthread_rwlock_unlock(rwlock)
#define ccds_rwlock_wunlock(rwlock)     pthread_rwlock_unlock(rwlock)


typedef pthread_mutex_t ccds_mtx;

#define ccds_mtx_init(mtx)              pthread_mutex_init(mtx, NULL)
#define ccds_mtx_destroy(mtx)           pthread_mutex_destroy(mtx)
#define ccds_mtx_lock(mtx)              pthread_mutex_lock(mtx)
#define ccds_mtx_unlock(mtx)            pthread_mutex_unlock(mtx)

#endif
