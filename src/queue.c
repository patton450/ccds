#include "queue.h"
queue * queue_new(memcfg * mem, ccds_error * e) {
    queue * q = memcfg_malloc(mem, sizeof(queue));
    if(q == NULL){
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }
    
    q->rep = list_new(QUEUE_DLEN, mem, e);
    if(q->rep == NULL) {
        memcfg_free(mem, q);
        return NULL;
    }
    
    q->mem = mem;
    return q;
}

void queue_free(queue * q, ccds_error * e){
    if(q == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }

    list_free(q->rep, e);
    memcfg_free(q->mem, q);
    CCDS_SET_ERR(e, CCDS_EOK);
}

size_t queue_length(queue * q, ccds_error * e){
    if(q == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return 0;
    }

    return list_length(q->rep, e);
}

void * queue_peek(queue * q, ccds_error * e){
    if(q == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }

    return list_get_head(q->rep, e);
}

void * queue_dequeue(queue * q, ccds_error * e){
    if(q == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }

    return list_remove_head(q->rep, e);
}

bool queue_enqueue(queue * q, void * data, ccds_error * e){
    if(q == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }

    return list_add_tail(q->rep, data, e);
}

