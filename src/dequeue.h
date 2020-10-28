#ifndef CCDS_DEQUEUE_H
#define CCDS_DEQUEUE_H

#include "list.h"
struct _dequeue {
    list *      rep;
};

typedef struct _dequeue dequeue;

dequeue *   dequeue_new(memcfg * mem, ccds_error * e);

void    dequeue_free(dequeue * dq, ccds_error * e);
size_t  dequeue_length(dequeue * dq, ccds_error * e);

void    dequeue_enqueue_front(dequeue * dq, void * data, ccds_error * e);
void    dequeue_enqueue_back(dequeue * dq, void * data, ccds_error * e);

void *  dequeue_dequeue_front(dequeue * dq, ccds_error * e);
void *  dequeue_dequeue_back(dequeue * dq, ccds_error * e);

#endif
