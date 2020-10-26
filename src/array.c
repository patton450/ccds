#include "array.h"

array * array_new(size_t cap, memcfg * m, ccds_error * e) {  
    /* Allocate pointers needed for the array, set errors if we encounter any */
    array * a = memcfg_malloc(m, sizeof(array));
    if(a == NULL) {
        log_error("Array failed to allocate");
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }
    
    a->buffer = memcfg_calloc(m, cap, sizeof(void *));
    log_debug("a->buffer = %p", a->buffer);
    if(a->buffer == NULL) {
        log_error("Buffer failed to allocate");
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }

    a->buff_lock = memcfg_malloc(m, sizeof(ccds_rwlock));
    if(a->buff_lock == NULL) {
        log_error("Buffer rwlock failed to allocate");
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }
    
    /* Initalize the rwlock and check errors */
    int tmp = ccds_rwlock_init(a->buff_lock);
    if(tmp != 0){
        log_error("Buffer rwlock failed to initalize with code %d", tmp);
        switch(tmp){
            case EAGAIN:
                CCDS_SET_ERR(e, CCDS_ERSC_FAIL);
                break;
            case ENOMEM:
                CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
                break;
            case EPERM:
                CCDS_SET_ERR(e, CCDS_EPERM);
                break;
            case EINVAL:
                CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
                break;
            case EBUSY:
                CCDS_SET_ERR(e, CCDS_EBUSY);
                break;
            default:
                log_fatal("rwlock init unknown error code %d", tmp);
                break;
        }
        return NULL;
    }
    
    a->capacity = cap;
    a->mem = m;
    
    CCDS_SET_ERR(e, CCDS_EOK);
    return a;
}

void array_free(array * a, ccds_error * e){
    if(a == NULL) {
        log_error("NULL array passed into array_free");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }

    int tmp = ccds_rwlock_destroy(a->buff_lock);
    if(tmp != 0) {
        log_error("Buffer rwlock failed to be destroyed with code %d", tmp);
        switch(tmp){
            case EINVAL:
                CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
                break;
            case EBUSY:
                CCDS_SET_ERR(e, CCDS_EBUSY);
                break;
            default:
                log_fatal("rwlock destroy unknown error code %d", tmp);
                break;
        }
        return;
    }
    
    memcfg_free(a->mem, a->buff_lock);
    memcfg_free(a->mem, a->buffer);
    memcfg_free(a->mem, a);
    
    CCDS_SET_ERR(e, CCDS_EOK);
}

size_t array_length(array * a, ccds_error * e) {
    if(a == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        log_error("NULL array passed into array_length");
        return 0;
    }

    CCDS_SET_ERR(e, CCDS_EOK);
    return a->capacity;
}

void * array_get(array * a, size_t indx, ccds_error * e){ 
    ccds_rwlock_rlock(a->buff_lock);
    
    if(a == NULL) { 
        ccds_rwlock_runlock(a->buff_lock);
        
        log_error("NULL array passed into array_get");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL; 
    }
    
    if(indx > a->capacity) {
        ccds_rwlock_runlock(a->buff_lock);
        
        log_error("Array index %lu, exceeds array capacity %lu", indx, a->capacity);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return NULL;
    }

    void * tmp = a->buffer[indx];  
    ccds_rwlock_runlock(a->buff_lock);

    CCDS_SET_ERR(e, CCDS_EOK);
    return tmp;
}


bool array_getn(array * a, size_t indx, void ** buff, size_t n, ccds_error * e){
    ccds_rwlock_rlock(a->buff_lock);
    
    if(a == NULL || buff == NULL) {
        ccds_rwlock_runlock(a->buff_lock);
        
        log_error("NULL %s passed into array_getn", (a == NULL ? "array" : "buffer"));
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }

    if(indx + n > a->capacity){ 
        ccds_rwlock_runlock(a->buff_lock);
       
        log_error("Copy boundry: %lu exceeds array capacity: %lu", indx + n, a->capacity);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return false;
    }

    for(size_t i = 0; i < n; i ++){
        buff[i] = a->buffer[indx + i];
    }
    ccds_rwlock_runlock(a->buff_lock);
    
    CCDS_SET_ERR(e, CCDS_EOK);
    return true;
}

void * array_set(array * a, size_t indx, void * data, ccds_error * e) {
    ccds_rwlock_wlock(a->buff_lock);
    
    if(a == NULL) {
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("NULL array passed into array_set");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL; 
    }

    if(indx >= a->capacity) {
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("Array index %lu, exceeds array capacity %lu", indx, a->capacity);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return NULL;
    }

    void * tmp = a->buffer[indx];
    a->buffer[indx] = data;

    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
    return tmp;
}

bool array_setn(array * a, size_t indx, void ** buff, size_t n, ccds_error * e){ 
    ccds_rwlock_wlock(a->buff_lock);
    
    if(a == NULL) {
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("NULL array passed into array_setn");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false; 
    }

    if(indx + n > a->capacity) {
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("writing boundry %lu, exceeds array capacity %lu", indx + n, a->capacity);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return false;
    }

    for(size_t i = 0; i < n; i ++) {
        void * tmp = a->buffer[indx + i];
        a->buffer[indx + i] = buff[i];
        buff[i] = tmp;
    }
    
    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
    return true;
}


bool array_resize(array * a, size_t cap, ccds_error * e) { 
    ccds_rwlock_wlock(a->buff_lock);
    
    if(a == NULL) { 
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("NULL array passed into array_resize");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL; 
    }
    
    // Array size not changed
    if(a->capacity == cap) {
        ccds_rwlock_wunlock(a->buff_lock);
        CCDS_SET_ERR(e, CCDS_EOK);
        return true;
    } 

    void ** tmp = memcfg_realloc(a->mem, a->buffer, cap * sizeof(void *));

    if(tmp == NULL) {
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("Failed to reallocate arrary from %lu to %lu elems", a->capacity, cap);
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return false;
    }
    
    a->buffer = tmp;
    a->capacity = cap;

    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
    return true;
}


bool array_insert_shift( array * a, size_t indx, void ** buff, size_t n, ccds_error * e){ 
    ccds_rwlock_wlock(a->buff_lock);
    if(a == NULL) {
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("NULL array passed into array_shiftr_fill");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    if(indx >= a->capacity - 1){ 
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("Index: %lu equals or exceeds shift capacity: %lu", indx, a->capacity - 1);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return false;
    }

    if(indx + n > a->capacity) {
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("Write boundry: %lu exceeds capacity: %lu", indx +  n, a->capacity);
        CCDS_SET_ERR(e, CCDS_EPRE_COND);
        return false;
    }


    void * tmp[n];
    for(size_t i = 0; i < n; i++){
        tmp[i] = a->buffer[(a->capacity - n - i)];
    }

    memmove(&a->buffer[indx + n], &a->buffer[indx], (a->capacity - n - indx) * sizeof(void *));

    for(size_t i = 0; i < n; i ++){
        if(buff == NULL){
            a->buffer[indx + i] = NULL;
        } else {
            a->buffer[indx + i] = buff[i];
            buff[i] = tmp[i];
        }
    }
    
    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
    return true;
}

bool array_remove_shift(array * a, size_t indx, void ** buff, size_t n, ccds_error * e){
    ccds_rwlock_wlock(a->buff_lock);
    if(a == NULL) {
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("NULL array passed into array_shiftr_fill");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }

    if(indx > a->capacity){ 
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("Index: %lu exceeds capacity: %lu", indx, a->capacity);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return false;
    }

    if( indx + n >= a->capacity) {
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("Write boundry: %lu exceeds capacity: %lu", indx - n, a->capacity);
        CCDS_SET_ERR(e, CCDS_EPRE_COND);
        return false;
    }

    void * tmp[n];
    for(size_t i = 0; i < n; i++){
        tmp[i] = a->buffer[indx];
    }

    memmove(&a->buffer[indx], &a->buffer[indx + n], (a->capacity - (indx + n)) * sizeof(void *));
   
    for(size_t i = 0; i < n; i ++) {
        if(buff == NULL){
            a->buffer[a->capacity - 1 - i] = NULL;
        } else {
            a->buffer[a->capacity - 1 - i] = buff[i];
        }
        buff[i] = tmp[i];
    }
    
    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
    return true;
}

bool array_swap(array * a, size_t indx1, size_t indx2, ccds_error * e){ 
    ccds_rwlock_wlock(a->buff_lock);
    
    if(a == NULL){
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("NULL array passed into array_swap");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    if(indx1 >= a->capacity || indx2 >= a->capacity){
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("Failed to swap indexs %lu %lu, one or both exceed %lu", indx1, indx2, a->capacity);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return false;
    }

    void * tmp = a->buffer[indx2];
    a->buffer[indx2] = a->buffer[indx1];
    a->buffer[indx1] = tmp; 
    
    ccds_rwlock_wunlock(a->buff_lock);

    CCDS_SET_ERR(e, CCDS_EOK);
    return true;
}

void array_foreach(array * a, void (*fn)(void **), ccds_error * e){
    ccds_rwlock_wlock(a->buff_lock);
    if(a == NULL){
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("NULL array passed into array_foreach");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }

    for(size_t i = 0; i < a->capacity; i++){
        fn(&(a->buffer[i]));
    }

    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
}

void array_foreachi(array * a, void (*fn)(void **, size_t), ccds_error * e){
    ccds_rwlock_wlock(a->buff_lock);

    if(a == NULL){
        ccds_rwlock_wunlock(a->buff_lock);
        
        log_error("NULL array passed into array_foreachi");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }

    for(size_t i = 0; i < a->capacity; i++){
        fn(&(a->buffer[i]), i);
    }

    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
}
