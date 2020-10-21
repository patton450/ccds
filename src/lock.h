#ifndef CCDS_LOCK_H
#define CCDS_LOCK_H


#include <pthread.h>

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
