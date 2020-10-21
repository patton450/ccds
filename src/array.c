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
        log_error("NULL array passed into array_get");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        ccds_rwlock_runlock(a->buff_lock);
        return NULL; 
    }
    
    if(indx >= a->capacity) {
        log_error("Array index %lu, exceeds array capacity %lu", indx, a->capacity);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        ccds_rwlock_runlock(a->buff_lock);
        return NULL;
    }

    void * tmp = a->buffer[indx];
   
    ccds_rwlock_runlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
    return tmp;
}

bool array_getn(array * a, size_t indx, void ** buffer, size_t n, ccds_error * e) {

}

void * array_set(array * a, size_t indx, void * data, ccds_error * e) {
    ccds_rwlock_wlock(a->buff_lock);
    
    if(a == NULL) {
        log_error("NULL array passed into array_set");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        ccds_rwlock_wunlock(a->buff_lock);
        return NULL; 
    }

    if(indx >= a->capacity) {
        log_error("Array index %lu, exceeds array capacity %lu", indx, a->capacity);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        ccds_rwlock_wunlock(a->buff_lock);
        return NULL;
    }

    void * tmp = a->buffer[indx];
    a->buffer[indx] = data;

    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
    return tmp;
}



bool array_resize(array * a, size_t cap, ccds_error * e) { 
    ccds_rwlock_wlock(a->buff_lock);
    
    if(a == NULL) { 
        log_error("NULL array passed into array_resize");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        ccds_rwlock_wunlock(a->buff_lock);
        return NULL; 
    }
    
    if(a->capacity == cap) {
        // Array size not changed
        ccds_rwlock_wunlock(a->buff_lock);
        return true;
    }

    void ** tmp = memcfg_realloc(a->mem, a->buffer, cap);
    if(tmp == NULL) {
        log_error("Failed to reallocate arrary from %lu to %lu elems", a->capacity, cap);
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        ccds_rwlock_wunlock(a->buff_lock);
        return false;
    }

    a->buffer = tmp;
    a->capacity = cap;

    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
    return true;
}

bool array_shiftr_fill(array * a, size_t indx, size_t off, void ** buff, size_t len, ccds_error * e) {
    ccds_rwlock_wlock(a->buff_lock);
    
    if(a == NULL) {
        log_error("NULL array passed into array_shiftr");
        ccds_rwlock_wunlock(a->buff_lock);
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }

    if(indx + off + len >= a->capacity) {
        log_error("Write boundry: %lu exceeds or equals capacity: %lu", indx + off + len, a->capacity);
        ccds_rwlock_wunlock(a->buff_lock);
        CCDS_SET_ERR(e, CCDS_EPRE_COND);
        return false;
    }
    
    void * tmp[buff_len];
    for(size_t i = 0; i < len; i++) {
        if(indx + off + i >= indx + len){
            tmp[i] = a->buffer[indx + off + i];
        }
    }

    /* Copy memory into new place */
    memcpy(&(a->buffer[indx + off]), &(a->buffer[indx]), len * sizeof(void *));    

    /* Buff is used to fill the newly opened spaces with pointers
        then tmp is emptied into buff so caller can access
        overwritten pointers */
    for(size_t i = 0; i < len; i ++) {
        a->buffer[indx + i] = buff[i];
        buff[i] = tmp[i]; 
    }

    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
    return true;
}

bool array_shiftl(array * a, size_t indx, size_t len, size_t off, void ** buff, size_t buff_len, ccds_error * e){
    ccds_rwlock_wlock(a->buff_lock); 
    if(indx < off || indx >= a->capacity) {
        log_error("Capacity: %lu, indx: %lu, off: %lu, buff_len: %lu", a->capacity, indx, off, buff_len);
        ccds_rwlock_wunlock(a->buff_lock);
        return false;
    }

    size_t num_copy = (indx - MIN(off, len)) + 1;
    log_trace("capacity: %lu, indx: %lu, off: %lu, buff_len: %lu, num_copy:%lu",
                a->capacity, indx, off, buff_len, num_copy);

    void * tmp[buff_len];
    for(size_t i = 0; i < off; i++){
        if(i < buff_len){
            tmp[i] = a->buffer[i];
        }
    }
    
    memcpy(&(a->buffer[a->capacity - len - indx - off]), &(a->buffer[off]), num_copy * sizeof(void *));
    
    for(size_t i = 0; i < off; i ++) {
        if(i < buff_len) {
            a->buffer[indx - i] = buff[i];
            buff[i] = tmp[i];
        }
    }

    ccds_rwlock_wunlock(a->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
    return true;
}

bool array_swap(array * a, size_t indx1, size_t indx2, ccds_error * e){ 
    ccds_rwlock_wlock(a->buff_lock);
    
    if(indx1 >= a->capacity || indx2 >= a->capacity){
        log_error("Failed to swap indexs %lu %lu, one or both exceed %lu", indx1, indx2, a->capacity);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        ccds_rwlock_wunlock(a->buff_lock);
        return false;
    }

    void * tmp = a->buffer[indx2];
    a->buffer[indx2] = a->buffer[indx1];
    a->buffer[indx1] = tmp; 
    
    ccds_rwlock_wunlock(a->buff_lock);

    CCDS_SET_ERR(e, CCDS_EOK);
    return true;
}
