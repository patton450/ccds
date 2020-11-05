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


/*
NAME:
    queue_new
DESCRIPTION:
    Allocates space on the head for a struct _queue. If mem != NULL then the funciton
        will allocate using the mem->*alloc funcitons, or ccds_d*alloc (see mem.h).
        If mem == NULL then ccds_d*alloc funcitons are used.
PARAMETERS:
    mem:    Either a pointer to a user defined memcfg or NULL
    e:      Pointer to an error enum, if e != NULL then the error will be set accordingly
                otherwise e is NULL, and no errors are set
RETURNS:
    queue *:    If the function completed without error
    NULL:       The function encountered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
    CCDS_EMEM_FAIL: One of the calls to allocate memory failed
*/
queue * queue_new(memcfg* mem, ccds_error * e);

/*
NAME:
    queue_free
DESCRIPTION:
    Frees a queue that was allocated using queue_new
PARAMETERS:
    q:      Pointer to the queue we are free
    e:      Pointer to an error enum, if e != NULL then the error will be set accordingly
                otherwise e is NULL, and no errors are set
ERRORS:
    CCDS_EOK: The funciton completed without error
    CCDS_EMEM_FAIL: One of the calls to allocate memory failed
*/
void    queue_free(queue * q, ccds_error * e);

/*
NAME:
    queue_length
DESCRIPTION:
    Returns the current number of elements in the queue
PARAMETERS:
    q:      Pointer to the queue that we will use
    e:      Pointer to an error enum, if e != NULL then the error will be set accordingly
                otherwise e is NULL, and no errors are set
RETURNS:
    size_t:     If the funciton completes without error, the lenght of the queue.
                    Note: length >= 0, check error pointer
    0:          An error occures
ERRORS:
    CCDS_EOK: The funciton completed without error
    CCDS_EMEM_FAIL: One of the calls to allocate memory failed
*/
size_t  queue_length(queue * q, ccds_error * e);

/*
NAME:
    queue_peek
DESCRIPTION:
    Returns the void pointer at the front of the queue without rempoving it
PARAMETERS:
    q:      Pointer to the queue we are inspecting data from
    e:      Pointer to an error enum, if e != NULL then the error will be set accordingly
                otherwise e is NULL, and no errors are set
RETURNS:
    void *:     Element at the front of the queue
    NULL:       The function encountered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
    CCDS_EMEM_FAIL: One of the calls to allocate memory failed
*/
void *  queue_peek(queue * q, ccds_error * e);

/*
NAME:
    queue_dequeue
DESCRIPTION:
    Removes the element at the front of the queue
PARAMETERS:
    q:      Pointer to the queue we are remvoing data from
    e:      Pointer to an error enum, if e != NULL then the error will be set accordingly
                otherwise e is NULL, and no errors are set
RETURNS:
    void *:     The element that was at the front of the list before the funciton call
    NULL:       The function encountered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
    CCDS_EMEM_FAIL: One of the calls to allocate memory failed
*/
void *  queue_dequeue(queue * q, ccds_error * e);

/*
NAME:
    queue_enqueue
DESCRIPTION:
    Adds the element data to the end of the queue q
PARAMETERS:
    q:      Pointer to the queue we are adding data to
    data:   Void pointer that will be added to the queue
    e:      Pointer to an error enum, if e != NULL then the error will be set accordingly
                otherwise e is NULL, and no errors are set
RETURNS:
    true:   The funciton completes without error
    false:  The function encountered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
    CCDS_EMEM_FAIL: One of the calls to allocate memory failed
*/
bool    queue_enqueue(queue * q, void * data, ccds_error * e);
#endif
