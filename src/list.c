#include "include/list.h"

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


    ccds_mtx_init(&(l->expand));
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

    ccds_mtx_destroy(&(l->expand));
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
    
    void * tmp[1] = { 0 };
    tmp[0] = data;
    ccds_mtx_lock(&(l->expand));
    if(l->length == l->buffer->capacity) {
        log_trace("Expanding the buffer from %lu to %lu", l->length, l->length * 2);
        if(!array_resize(l->buffer, l->length * 2, e)) {
            ccds_mtx_unlock(&(l->expand));
            return false;
        }
    }
    ccds_mtx_unlock(&(l->expand));
    return array_insert_shift(l->buffer, l->length++, tmp, 1, e); 
}

bool list_add_head(list * l, void * data, ccds_error * e) { 
    if(l == NULL) {
        log_error("NULL list passed to list_add_tail");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    void * tmp[1] = { 0 };
    tmp[0] = data;
    ccds_mtx_lock(&(l->expand));
    if(l->length == l->buffer->capacity) {
        log_trace("Expanding the buffer from %lu to %lu", l->length, l->length * 2);
        if(!array_resize(l->buffer, l->length * 2, e)) {
            ccds_mtx_unlock(&(l->expand));
            return false;
        }
    }
    ccds_mtx_unlock(&(l->expand));
    l->length++; 
    return array_insert_shift(l->buffer, 0, tmp, 1, e); 
}

bool list_add_tail(list * l, void * data, ccds_error * e) {
    if(l == NULL) {
        log_error("NULL list passed to list_add_tail");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    ccds_mtx_lock(&(l->expand));
    if(l->length == l->buffer->capacity) {
        log_trace("Expanding the buffer from %lu to %lu", l->length, l->length * 2);
        if(!array_resize(l->buffer, l->length * 2, e)) {
            ccds_mtx_unlock(&(l->expand));
            return false;
        }
    }
    ccds_mtx_unlock(&(l->expand));
    size_t len = l->length++;
    
    void * tmp[1] = { 0 };
    tmp[0] = data;
    return array_insert_shift(l->buffer, len, tmp, 1, e); 
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
        log_warn("index: %lu exceeds or equals list length: %lu", indx, l->length);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return NULL;
    }
    
    l->length--;
    void * tmp[1] = { NULL };
    array_remove_shift(l->buffer, indx, tmp, 1, e);
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
        log_warn("Cannot remove the tail of an empty list");
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
        log_error("Either indx1: %lu or indx2: %lu exceed or equal list length: %lu", 
			indx1, indx2, l->length);
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return false;
    }

    return array_swap(l->buffer, indx1, indx2, e);
}


void list_foreach(list * l, void (*fn)(void **), ccds_error * e){ 
    
    if(l == NULL) { 
        log_error("NULL list passed to list_foreach");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }
    
    array * a = l->buffer;
    if(a == NULL) {     
        log_error("NULL array passed to list_map");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }
    
    ccds_rwlock_wlock(&(a->buff_lock));
    for(size_t i = 0; i < l->length; i++){
        if(a && a->buffer && a->buffer[i]) {
            fn(&(a->buffer[i]));
        }
    }

    ccds_rwlock_wunlock(&(a->buff_lock)); 
    CCDS_SET_ERR(e, CCDS_EOK);
}

void list_foreachi(list * l, void (*fn)(void **, size_t), ccds_error * e){ 
    if(l == NULL) {     
        log_error("NULL list passed to list_foreach");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }
 
    array * a = l->buffer;
    if(a == NULL) { 
        log_error("NULL array passed to list_map");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }
    
    ccds_rwlock_wlock(&(a->buff_lock));     
    for(size_t i = 0; i < l->length; i++){
        if(a && a->buffer && a->buffer[i]) {
            fn(&(a->buffer[i]), i);
        }
    }
    ccds_rwlock_wunlock(&(a->buff_lock));
    CCDS_SET_ERR(e, CCDS_EOK);
}


void * list_foldl(list * l, void * start, void (*fn)(void *, void *), ccds_error * e){
    if(l == NULL) {     
        log_error("NULL list passed to list_foldl");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }
    
    array * a = l->buffer;
    if(a == NULL) {     
        log_error("NULL array passed to list_map");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }
    
    ccds_rwlock_rlock(&(a->buff_lock));
    for(size_t i = 0; i < l->length; i ++) {
        if(a && a->buffer[i])
        fn(start, a->buffer[i]);
    }
    ccds_rwlock_runlock(&(a->buff_lock));
    
    CCDS_SET_ERR(e, CCDS_EOK);
    return start;
}

void * list_foldr(list * l, void * start, void (*fn)(void *, void *), ccds_error * e){
    if(l == NULL) {     
        log_error("NULL list passed to list_foldr");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }
    
    array * a = l->buffer;
    if(a == NULL) {     
        log_error("NULL array passed to list_map");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }
    
    ccds_rwlock_rlock(&(a->buff_lock));
    for(size_t i = l->length - 1; i > 0; i--) {
        fn(a->buffer[i], start);
    }
    fn(a->buffer[0], start);
    ccds_rwlock_runlock(&(a->buff_lock));

    CCDS_SET_ERR(e, CCDS_EOK);
    return start;
}

void list_map (list * l, void ** buff, size_t buff_len, void (*fn) (void *, void **), ccds_error * e){    
    if(l == NULL) {     
        log_error("NULL list passed to list_map");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }

    if(l->buffer == NULL) {     
        log_error("NULL array passed to list_map");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }
    
    array * a = l->buffer;
    ccds_rwlock_rlock(&(a->buff_lock));
    for(size_t i = 0; i < l->length && i < buff_len; i++) {
        fn(a->buffer[i], &buff[i]);
    }
    ccds_rwlock_runlock(&(a->buff_lock));
    CCDS_SET_ERR(e, CCDS_EOK);
}

void list_filter(list * l, void ** buff, size_t buff_len, bool (*fn) (void *), ccds_error * e){
    if(l == NULL) {     
        log_error("NULL list passed to list_filter");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }

    array * a = l->buffer;
    if(a == NULL) {     
        log_error("NULL array passed to list_map");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }

    size_t j = 0;
    ccds_rwlock_rlock(&(a->buff_lock));
    for(size_t i = 0; i < l->length; i++) {
        if(i < buff_len && fn(a->buffer[i])){
            buff[j++] = a->buffer[i];
        }
    }
    ccds_rwlock_runlock(&(a->buff_lock));
    CCDS_SET_ERR(e, CCDS_EOK);
}

bool list_any(list * l, bool (*fn) (void *), ccds_error * e){
    if(l == NULL) {     
        log_error("NULL list passed to list_any");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }

    array * a = l->buffer;
    if(a == NULL) {     
        log_error("NULL array passed to list_map");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    bool any = false;
    ccds_rwlock_rlock(&(a->buff_lock));
    for(size_t i = 0; i < l->length && !any; i++) {
        any |= fn(a->buffer[i]);
    }

    ccds_rwlock_runlock(&(a->buff_lock));
    CCDS_SET_ERR(e, CCDS_EOK);

    return any;
}

bool list_all(list * l, bool (*fn) (void *), ccds_error * e){
    if(l == NULL) {     
        log_error("NULL list passed to list_all");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }

    array * a = l->buffer;
    if(a == NULL) {     
        log_error("NULL array passed to list_map");
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }

    bool all = true;
    ccds_rwlock_rlock(&(a->buff_lock));
    for(size_t i = 0; i < l->length && all; i++) {
        all &= fn(a->buffer[i]);
    }

    ccds_rwlock_runlock(&(a->buff_lock));
    CCDS_SET_ERR(e, CCDS_EOK);

    return all;
}
