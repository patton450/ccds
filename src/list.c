#include "list.h"
list * list_new(size_t cap, memcfg * mem, ccds_error * e) {
    list * l = memcfg_malloc(mem, sizeof(list));
    if(l == NULL) {
        log_error("list failed to malloc");
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }

    l->buffer = array_new(cap, mem, e);
    if(l->buffer == NULL) {
        memcfg_free(l->mem, l);
        log_error("array failed to allocate");
        return NULL;
    }

    l->expand = memcfg_malloc(mem, sizeof(ccds_mtx));
    if(l->expand == NULL) { 
        array_free(l->buffer, e);
        memcfg_free(l->mem, l);
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        log_error("expand mutex failed to allocate");
        return NULL;
    }

    l->mem = mem;
    l->length = 0;

    CCDS_SET_ERR(e, CCDS_EOK);
    return l;
}

void list_free(list * l, ccds_error * e) {
    if(l == NULL) {
        log_error("NULL list passed to list_free");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }

    array_free(l->buffer, e);
    memcfg_free(l->mem, l);
    CCDS_SET_ERR(e, CCDS_EOK);
}

size_t list_length(list * l, ccds_error * e){
    if(l == NULL) {
        log_error("NULL list passed to list_length");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return 0;
    }

    CCDS_SET_ERR(e, CCDS_EOK);
    return l->length;
}

/* Adding elements */
bool list_add(list * l, size_t indx, void * data, ccds_error * e){
    if(l == NULL) {
        log_error("NULL list passed to list_add");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }

    if(indx > l->length) {
        log_error("Index %lu exceeds list bounds %lu", indx, l->length);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return false;
    }

    ccds_mtx_lock(l->expand);
    if(l->length == l->buffer->capacity) {
        log_trace("Expanding the buffer from %lu to %lu", l->length, l->length * 2);
        if(!array_resize(l->buffer, l->length * 2, e)) {
            ccds_mtx_unlock(l->expand);
            return false;
        }
    }
    
    size_t len = l->length++;
    ccds_mtx_unlock(l->expand);

    size_t num = MAX(1, len - indx);
    size_t off = len != indx ? 1 : 0;

    void * tmp[num];
    memset(tmp, 0, num);
    tmp[0] = data;

    return array_shiftr_fill(l->buffer, indx, off, tmp, num, e); 
}

bool list_add_head(list * l, void * data, ccds_error * e){
    return list_add(l, 0, data, e);
}

bool list_add_tail(list * l, void * data, ccds_error * e) {
    if(l == NULL) {
        log_error("NULL list passed to list_add_tail");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    return list_add(l, l->length, data, e);
}

/* Accessing elements */
void * list_get(list * l, size_t indx, ccds_error * e){
    if(l == NULL) {
        log_error("NULL list passed to list_get");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }

    if(indx >= l->length) {
        log_error("index: %lu exceeds or equals list length: %lu", indx, l->length);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return NULL;
    }

    return array_get(l->buffer, indx, e);
}

void * list_get_head(list * l, ccds_error * e) {
    return list_get(l, 0, e);
}

void * list_get_tail(list * l, ccds_error * e ){
    if(l == NULL) {
        log_error("NULL list passed to list_get_tail");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    if(l->length == 0) {
        log_error("Cannot get the tail of an empty list");
        CCDS_SET_ERR(e, CCDS_EPRE_COND);
        return NULL;
    }
    
    return list_get(l, l->length - 1, e);
}


/* Removing elements */
void * list_remove(list * l, size_t indx, ccds_error * e){
    if(l == NULL) {
        log_error("NULL list passed to list_remove");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    if(indx >= l->length) {
        log_error("index: %lu exceeds or equals list length: %lu", indx, l->length);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return NULL;
    }
    
    size_t len = l->length --;
    size_t num = MAX(1, len - indx);
    size_t off = 1; 

    void * tmp[num];
    memset(tmp, 0, num);

    array_shiftl_fill(l->buffer, len - indx, off, tmp, num, e);

    return tmp[0];
}

void * list_remove_head(list * l, ccds_error * e){
    return list_remove(l, 0, e);
}

void * list_remove_tail(list * l, ccds_error * e) {
    if(l == NULL) {
        log_error("NULL list passed to list_remove_tail");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    if(l->length == 0) {
        log_error("Cannot remove the tail of an empty list");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }
    
    return list_remove(l, l->length - 1, e);
}

bool list_swap(list * l, size_t indx1, size_t indx2, ccds_error * e){
    if(l == NULL){
        log_error("NULL list passed to list_swap");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    if(indx1 >= l->length || indx2 >= l->length){
        log_error("Either indx1: %lu or indx2: %lu exceed or equal list length: %lu", indx1, indx2, l->length);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return false;
    }

    return array_swap(l->buffer, indx1, indx2, e);
}


void list_foreach(list * l, void (*fn)(void **), ccds_error * e){ 
    ccds_rwlock_wlock(l->buffer->buff_lock);
    
    if(l == NULL) {
        ccds_rwlock_wunlock(l->buffer->buff_lock);
        log_error("NULL list passed to list_foreach");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    for(size_t i = 0; i < l->length; i++){
        fn(&(f->buffer->buffer[i]));
    }

    ccds_rwlock_wunlock(l->buffer->buff_lock); 
    CCDS_SET_ERR(e, CCDS_EOK);
}

void list_foreachi(list * l, void (*fn)(void **, size_t), ccds_error * e){ 
    ccds_rwlock_wlock(l->buffer->buff_lock);
    
    if(l == NULL) {
        ccds_rwlock_wunlock(l->buffer->buff_lock);
        log_error("NULL list passed to list_foreach");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    for(size_t i = 0; i < l->length; i++){
        fn(&(f->buffer->buffer[i]), i);
    }

    ccds_rwlock_wunlock(l->buffer->buff_lock);
    CCDS_SET_ERR(e, CCDS_EOK);
}


void * list_foldl(list * l, void * start, void * (*fn)(void *, const void *), ccds_error * e){

}

void * list_foldr(list * l, void * start, void * (*fn)(const void *, void *), ccds_error * e){

}
