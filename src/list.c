#include "list.h"

list * list_new(size_t cap, memcfg * mem, ccds_error * e){
    list * l = memcfg_malloc(mem, sizeof(list));
    if(!l) {
        log_error("list failed to malloc");
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }

    l->buffer = array_new(cap, mem, e);
    if(!l->buffer) {
        memcfg_free(l->mem, l);
        log_error("array failed to allocate");
        return NULL;
    }

    l->expand = memcfg_malloc(mem, sizeof(ccds_mtx));
    if(!l->expand) { 
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
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }

    array_free(l->buffer, e);
    memcfg_free(l->mem, l);
    CCDS_SET_ERR(e, CCDS_EOK);
}

size_t list_length(list * l, ccds_error * e){
    if(l == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return 0;
    }

    CCDS_SET_ERR(e, CCDS_EOK);
    return l->length;
}

/* Adding elements */
bool list_add(list * l, size_t indx, void * data, ccds_error * e){
    if(l == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }

    if(indx > l->length) {
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return false;
    }

    ccds_mtx_lock(l->expand);
    if(l->length == l->buffer->capacity) {
        if(!array_resize(l->buffer, l->length * 2, e)){
            CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        }
    }
    ccds_mtx_unlock(l->expand);  
    void * tmp[1] = { data };

    size_t num = (l->length - indx) == 0 ? 1 : l->length - indx;
    l->length++;

    return array_shiftr(l->buffer, indx, num, 1, tmp, 1, e); 
}

bool list_add_head(list * l, void * data, ccds_error * e){
    return list_add(l, 0, data, e);
}

bool list_add_tail(list * l, void * data, ccds_error * e) {
    if(l == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    return list_add(l, l->length, data, e);
}

/* Accessing elements */
void * list_get(list * l, size_t indx, ccds_error * e){
    if(l == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }

    if(indx >= l->length) {
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return NULL;
    }
    return array_get(l->buffer, indx, e);
}

void * list_get_head(list * l, ccds_error * e) {
    return list_get(l, 0, e);
}

void * list_get_tail(list * l, ccds_error * e ){
    if(l == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    if(l->length == 0) {
        CCDS_SET_ERR(e, CCDS_EPRE_COND);
        return NULL;
    }
    return list_get(l, l->length - 1, e);
}


/* Removing elements */
void * list_remove(list * l, size_t indx, ccds_error * e){
    if(l == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    if(indx >= l->length) {
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return NULL;
    }
    
    void * tmp[1] = { NULL };
    size_t num = (l->length - indx) == 0 ? 1 : l->length - indx;
    array_shiftl(l->buffer, indx, num, 1, tmp, 1, e);
    l->length --;

    return tmp[0];
}

void * list_remove_head(list * l, ccds_error * e){
    return list_remove(l, 0, e);
}

void * list_remove_tail(list * l, ccds_error * e) {
    if(l->length == 0) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }
    
    return list_remove(l, l->length, e);
}

bool list_swap(list * l, size_t indx1, size_t indx2, ccds_error * e){
    if(l == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    
    if(indx1 >= l->length || indx2 >= l->length){
        CCDS_SET_ERR(e, CCDS_EINDX_OB);
        return false;
    }

    return array_swap(l->buffer, indx1, indx2, e);
}
