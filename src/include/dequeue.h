#ifndef CCDS_DEQUEUE_H
#define CCDS_DEQUEUE_H

#include "list.h"
#include "mem.h"
#include "error.h"
#include "log.h"

struct _dequeue {
    list *      rep;
    memcfg *    mem;
};

#define DEQUEUE_DLEN 100
typedef struct _dequeue dequeue;


/*
NAME:
    dequeue_new
DESCRIPTION:
    Allocates space for a new double-ended queue
PARAMETERS:
    mem:    Pointer to a memcfg or NULL
    e:      Pointer to a ccds_error or NULL
RETURNS:
    dequeue *:  If the function completes succesfully
    NULL:       If the funciton errors
ERRORS:
    CCDS_EOK: If the fucntion completes without error
    CCDS_EMEM_FAIL: If the function encounters an error during one of the allocations
*/
dequeue *   dequeue_new(memcfg * mem, ccds_error * e);

/*
NAME:
DESCRIPTION:
PARAMETERS:
RETURNS:
ERRORS:
*/
void    dequeue_free(dequeue * dq, ccds_error * e);

/*
NAME:
DESCRIPTION:
PARAMETERS:
RETURNS:
ERRORS:
*/
size_t  dequeue_length(dequeue * dq, ccds_error * e);


/*
NAME:
DESCRIPTION:
PARAMETERS:
RETURNS:
ERRORS:
*/
bool    dequeue_enqueue_front(dequeue * dq, void * data, ccds_error * e);

/*
NAME:
DESCRIPTION:
PARAMETERS:
RETURNS:
ERRORS:
*/
bool    dequeue_enqueue_back(dequeue * dq, void * data, ccds_error * e);


/*
NAME:
DESCRIPTION:
PARAMETERS:
RETURNS:
ERRORS:
*/
void *  dequeue_dequeue_front(dequeue * dq, ccds_error * e);

/*
NAME:
DESCRIPTION:
PARAMETERS:
RETURNS:
ERRORS:
*/
void *  dequeue_dequeue_back(dequeue * dq, ccds_error * e);

#endif
