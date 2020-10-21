#ifndef CCDS_QUEUE_H 
#define CCDS_QUEUE_H

#define QUEUE_DLEN 100  //Length of the list that the queue uses

#include "list.h"
#include "error.h"
#include "mem.h"

struct _queue {
    list * rep;
    memcfg * mem;
};

typedef struct _queue queue;

queue * queue_new(memcfg* mem, ccds_error * e);
void    queue_free(queue * q, ccds_error * e);
size_t  queue_length(queue * q, ccds_error * e);

void *  queue_peek(queue * q, ccds_error * e);
void *  queue_dequeue(queue * q, ccds_error * e);
bool    queue_enqueue(queue * q, void * data, ccds_error * e);

#endif
