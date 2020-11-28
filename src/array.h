#ifndef CCDS_ARRAY_H
#define CCDS_ARRAY_H

#include <stdbool.h>
#include <stdatomic.h>

#include "mem.h"
#include "util.h"
#include "lock.h"
#include "error.h"
#include "log.h"

#if __STDC_NO_ATOMICS__ 
    /* If there are no atomics then the implementation should
        handle a similar object since c11 right? */
#else
#endif

/*struct for thread safe access */
struct _array {
    atomic_size_t   capacity;   /* atomic size_t so no race conditions  */
    void **         buffer;     /* Have to use RWLock to keep in sync   */
    memcfg *        mem;        /* User defined memory mangement        */
    ccds_rwlock     buff_lock;  /* Reader-Writer lock for the buffer;   */
};
typedef struct _array array;


/* 
NAME:
    array_new
DESCRIPTION:
    Allocates space on the heap for a struct _array, if mem != NULL, then
        the function will allocate using mem->*alloc or ccds_d*alloc (see mem.h)
        and set itself as the memcfg for the array. If mem == NULL, then 
        the memory defaults are used ccds_d*alloc (see mem.h). Capacity is the initial 
        capacity of the array note this function allocates (cap * sizeof(void *)) bytes.
PARAMETERS:
    cap:    Initial array capacity
    mem:    Either a pointer to a memcfg, or NULL if defaults are to be used
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    array *: If we successfully allocate everything needed for the array
    NULL: If the function encounters an error
ERRORS:
    CCDS_EOK: The function completed without error
    CCDS_EMEM_FAIL: One of the calls to allocate memory failed
*/
array * array_new(size_t cap, memcfg * mem, ccds_error * e);


/* 
NAME:
    array_free
DESCRIPTION:
    Frees memory allocated to a struct _array by the function array_new.
PARAMETERS:
    a:      Pointer to an array to be freed 
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
ERRORS:
    CCDS_EINVLD_PARAM: If a == NULL, or ccds_rwlock_buffer returns INVLD
    CCDS_EBUSY: If ccds_rwlock_buffer returns EBUSY
    CCDS_EOK: The function completed without error
*/
void    array_free(array * a, ccds_error * e);


/* 
NAME:
    array_length
DESCRIPTION:
    Returns the capacity of the array, to be consistent across all data structures
PARAMETERS:
    a:      Pointer to the array we are getting the capacity of 
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    size_t: a->capacity
ERRORS:
    CCDS_EINVLD_PARAM:  If a == NULL
    CCDS_EOK: The function completed without error
*/
size_t  array_length(array * a, ccds_error * e);


/* 
NAME:
    array_get
DESCRIPTION:
    Gets the element occupying the index
PARAMETERS:
    a:      Array we are trying to access
    indx:   Index we want to check
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set 
RETURNS:
    void *: The current value of the array at index
ERRORS:
    CCDS_EINDX_OB: indx >= a->capcity
    CCDS_EINVLF_PARAM: a is NULL
    CCDS_EOK: The function completed without error
*/
void *  array_get(array * a, size_t indx, ccds_error * e);


/* 
NAME:
    array_getn
DESCRIPTION:
    Copies from [a->buffer[indx], a->buffer[indx + n]] into buffer. Assumes buffer has a minimum
        of n * sizeof(void *) bytes allocated to it.
PARAMETERS:
    a:      Pointer to the array we are reading
    indx:   Starting index to copy n elements
    buffer: Array of void * that the results will be written
    n:      Number of elements we are copying and size of buffer
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set. 
RETURNS:
    true: If the function completes without error
    false: otherwise
ERRORS:
    CCDS_EINDX_OB: indx + n > a->capcity
    CCDS_EINVLD_PARAM: a is NULL, or buffer is NULL
    CCDS_EOK: The function completed without error
*/
bool    array_getn(array * a, size_t indx, void ** buffer, size_t n, ccds_error * e);


/* 
NAME:
    array_set
DESCRIPTION:
    Writes the specified element at the index into the array. Returns what the 
        void * at the index was before writing, or NULL if the function completes 
        with error.
PARAMETERS:
    a:      Pointer to the array we are writing to
    indx:   Index of a->buffer we are writing data into
    p:      Data that will occupy a->buffer[indx] after the function completes
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set
RETURNS:
    void *: the void pointer that used to occupy indx
ERRORS:
    CCDS_EINDX_OB: indx >= a->capcity
    CCDS_EINVLD_PARAM: a is NULL
    CCDS_EOK: The function completed without error
*/
void *  array_set(array * a, size_t indx, void * p, ccds_error * e);


/* 
NAME:
    array_setn
DESCRIPTION:
    Copies from buffer into [a->buffer[indx], a->buffer[indx + n]]. Assumes buffer has a minimum
        of n * sizeof(void *) bytes allocated to it. After the function call is complete buffer will 
        contain the elements that used to occupy [a->buffer[indx], a->buffer[indx + n - 1]].
PARAMETERS:
    a:      Pointer to the array we are writing to
    indx:   Starting index 
    buffer: Array that holds elems we are writing into the array
    n:      Number of elements we are writing
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true:   if the function completes without error
    false:  otherwise
ERRORS:
    CCDS_EINDX_OB: indx + n > a->capcity
    CCDS_EINVLD_PARAM: a is NULL or buffer is NULL
    CCDS_EOK: The function completed without error
*/
bool    array_setn(array * a, size_t indx, void ** buffer, size_t n, ccds_error * e);


/* 
NAME:
    array_set_if_null
DESCRIPTION:
    Sets the index of the array to data, if the array is null at that index before insertion
PARAMETERS:
    a:      Pointer to the array we are writing to
    indx:   Index we are trying to set 
    data:   Data we are trying to insert
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true:   if the function completes without error
    false:  otherwise
ERRORS:
    CCDS_EINDX_OB: indx >= a->capcity
    CCDS_EINVLD_PARAM: a is NULL or buffer is NULL
    CCDS_EOK: The function completed without error
*/
bool    array_set_if_null(array * a, size_t indx, void * data, ccds_error * e);

/* 
NAME:
    array_resize
DESCRIPTION:
    Resizes the array to the size specified 
PARAMETERS:
    a: the array we are resizing
    size: size we want the arrayto be
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
RETURNS:
    true:   if the function completes without error
    false:  otherwise
ERRORS:
    CCDS_EINVLD_PARAM: a is NULL
    CCDS_EMEM_FAIL: If memcfg_realloc returns NULL
    CCDS_EOK: The function completed without error
*/
bool    array_resize(array * a, size_t size, ccds_error * e);


/* 
NAME:
    array_insert_shift
DESCRIPTION:
    Copies from buffer into [a->buffer[indx], a->buffer[indx + n - 1]]. Elements from 
        [a->buffer[indx + n], a->buffer[a->capacity - n]] will be shifted by n places to the left.
        Buffer is written into [a->buffer[capacity - n], a->buffer[capacity - 1]]. After the function call
        the elements that previously occupied [a->buffer[a->capacity - n], a->buffer[a->capacity - 1]] 
        will occupy buffer.
PARAMETERS:
    a:      Array we are inserting into
    indx:   Index we will insert at
    buffer: Array of void pointers that will be written into array, if NULL, will write NULL values for all n
    n:      The number of elements we are writing, and the minimum size of buffer
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set 
RETURNS:
    true:   if the function completes without error
    false:  otherwise
ERRORS:
    CCDS_EINDV_PARAM: If a == NULL
    CCDS_EINDX_OB: If indx >= a->capacity - 1
    CCDS_EPRE_COND: If (indx + 2*n) > a->capacity
    CCDS_EOK: The function completed without error
*/

bool    array_insert_shift(array * a, size_t indx, void ** buffer, size_t n, ccds_error * e);


/* 
NAME:
    array_remove_shift
DESCRIPTION:
    Removes [a->buffer[indx], a->buffer[indx + n]]. Elements [a->buffer[indx + n], a->buffer[a->capacity - 1]]
        will be be shifted by n places to the left. The elements that occupy buffer will be written to
        [a->buffer[a->capacity - n], a->buffer[a->capacity]]. After the function call is complete
        buffer will contain the elements that previously occupied [a->buffer[indx], a->buffer[indx + n]]
PARAMETERS:
    a:      Array we are inserting into
    indx:   Index we will insert at
    buffer: Array of void pointers that will be written into array, if NULL, will write NULL values for all n
    n:      The number of elements we are writing, and the minimum size of buffer
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set 
RETURNS:
    true:   if the function completes without error
    false:  otherwise
ERRORS:
    CCDS_EINDV_PARAM: If a == NULL
    CCDS_EINDX_OB: If indx >= a->capacity - 1
    CCDS_EPRE_COND: If (indx + n) > a->capacity
    CCDS_EOK: The function completed without error
*/
bool    array_remove_shift(array * a, size_t indx, void ** buffer, size_t n, ccds_error * e);


/* 
NAME:
    array_swap
DESCRIPTION:
    Swaps elements at the two indices
PARAMETERS:
    a:      Array funciton operates on
    indx1:  First index
    index2: Second index
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set 
RETURNS:
    true:   if the function completes without error
    false:  otherwise
ERRORS:
    CCDS_EINDV_PARAM: If a == NULL
    CCDS_EINDX_OB: If indx1 > a->capacity or indx2 > a->capacity
    CCDS_EOK: The function completed without error
*/
bool    array_swap(array * a, size_t indx1, size_t indx2, ccds_error * e);


/*
NAME:
    array_swap_if
DESCRIPTION:
    Swaps elements at the two indices, if the comparison function returns true
PARAMETERS:
    a:      Array function operates on
    indx1:  First index
    index2: Second index
    cmp:    Comparison function that compares the elements at indx1 and indx2
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set 
RETURNS:
    true:   if the function completes without error
    false:  otherwise
ERRORS:
    CCDS_EINDV_PARAM: If a == NULL
    CCDS_EINDX_OB: If indx1 > a->capacity or indx2 > a->capacity
    CCDS_EOK: The function completed without error
*/
bool    array_swap_if(array * a, size_t indx1, size_t indx2, bool (*cmp) (void *, void *), ccds_error * e);


/* 
NAME:
    array_foreach
DESCRIPTION:
    Loops over every element passing a pointer to the current element to the function
        for user interaction. NOTE: if you need to manipulate the array don't use the 
        array methods as this function holds a write lock on a->buffer.
PARAMETERS:
    a:  Pointer to the array we are looping over
    fn: Pointer to caller's function that will be given pointers to the elements
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
ERRORS:
    CCDS_EINVLD_PARAM:  a == NULL
    CCDS_EOK: The function completed without error
*/
void    array_foreach(array * a, void (*fn)(void **), ccds_error * e);


/* 
NAME:
    array_foreachi
DESCRIPTION:
    Loops over every element passing a pointer to the current element, 
        and index to fthe funciton for user interaction. NOTE: if you need to manipulate 
        the array don't use the array methods as this function holds a write lock on a->buffer.
PARAMETERS:
    a:  Pointer to the array we are looping over
    fn: Pointer to caller's function that will be given pointers to the elements
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
ERRORS:
    CCDS_EINVLD_PARAM:  a == NULL
    CCDS_EOK: The function completed without error
*/
void    array_foreachi(array * a, void (*fn)(void **, size_t), ccds_error * e);
#endif
