#include "include/dequeue.h"

dequeue * dequeue_new(memcfg * mem, ccds_error * e) {
    dequeue * dq = memcfg_malloc(mem, sizeof(dequeue));
    if(dq == NULL) {
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }

    dq->rep = list_new(DEQUEUE_DLEN, mem, e);
    if(dq->rep == NULL) {
        memcfg_free(mem, dq);
        return NULL;
    }

    dq->mem = mem;
    CCDS_SET_ERR(e, CCDS_EOK);
    return dq;
}

void dequeue_free(dequeue * dq, ccds_error * e){
    if(dq == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }

    list_free(dq->rep, e);
    memcfg_free(dq->mem, dq);
}

size_t dequeue_length(dequeue * dq, ccds_error * e){
    if(dq == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return 0;
    }

    CCDS_SET_ERR(e, CCDS_EOK);
    return dq->rep->length;
}

bool dequeue_enqueue_front(dequeue * dq, void * data, ccds_error * e){
    if(dq == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return 0;
    }

    return list_add_head(dq->rep, data, e);
}

bool dequeue_enqueue_back(dequeue * dq, void * data, ccds_error * e){
    if(dq == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return 0;
    }

    return list_add_tail(dq->rep, data, e);
}

void * dequeue_dequeue_front(dequeue * dq, ccds_error * e){
    if(dq == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }

    return list_remove_head(dq->rep, e);
}


void * dequeue_dequeue_back(dequeue * dq, ccds_error * e){
    if(dq == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }

    return list_remove_tail(dq->rep, e);
}
