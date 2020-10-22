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
    //idk
#else
#endif

//struct for thread safe access
struct _array {
    atomic_size_t   capacity;   // atomic size_t so no race conditions
    void **         buffer;     // Have to use RWLock to keep in sync
    memcfg *        mem;        // User defined memory mangement
    ccds_rwlock*    buff_lock;  // Reader-Writer lock for the buffer;
};
typedef struct _array array;

/* 
NAME:
    array_new
DESCRIPTION:
    Allocates space on the heap for a struct _array, if mem != NULL then
        the funciton will allocate using mem->*alloc or ccds_d*alloc (see mem.h)
        and set itself as the memcfg for the array. If mem == NULL, then 
        the memory defaults are used ccds_d*alloc (see mem.h). Capacity is the inital 
        capacity of the array note this funciton allocates (cap * sizeof(void *)) bytes.
PARAMETERS:
    cap: Inital capacity to set the array to
    mem: Either a pointer to a memcfg, or NULL if defaults are to be used
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
RETURNS:
    array *: If we succesfully allocate everything needed for the array
    NULL: If the funciton encounters an error
ERRORS:
    CCDS_EOK: The function completed without error
    CCDS_EMEM_FAIL: One of the calls to allocate memory failed
*/
array * array_new(size_t cap, memcfg * mem, ccds_error * e);

/* 
NAME:
    array_free
DESCRIPTION:
    Frees memory allocated to a struct _array by the funciton array_new.
PARAMETERS:
    a: Pointer to an array to be freed 
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
ERRORS:
    CCDS_EOK: The function completed without error
*/
void    array_free(array * a, ccds_error * e);

/* 
NAME:
    array_length
DESCRIPTION:
    Returns the capacity of the array, to be consistent accross all data structures
PARAMETERS:
    a:  Pointer to the array we are getting the capacity of 
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
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
    Gets the element occupying indx
PARAMETERS:
    a: Array we are trying to access
    indx: Index we want to check
    e: Pointer to an error enum, if e != NULL then error is set accordingly. 
            Otherwise e is NULL, and no errors will be set 
RETURNS:
    void *: The current value of the array at indx
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
    Copies from the specified indx to indx + n. Assumes buffer has space
        for n void *'s and fills buffer with whats in the array.
PARAMETERS:
    a:  Pointer to the array we are reading from
    indx:   Starting index to copy from
    buffer: Array that the results will be written into
    n:  Number of elements we are copying and size of buffer
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
RETURNS:
    true: If the function completes without error
    false: otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    array_getn(array * a, size_t indx, void ** buffer, size_t n, ccds_error * e);

/* 
NAME:
    array_set
DESCRIPTION:
    Writes the specified element at indx in the array, returns what the 
        void * at indx was before writing.
PARAMETERS:
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
RETURNS:
    void *: the void pointer that used to occupy indx
ERRORS:
    CCDS_EOK: The function completed without error
*/
void *  array_set(array * a, size_t indx, void * p, ccds_error * e);

/* 
NAME:
    array_setn
DESCRIPTION:
    Writes the number of elements specified from buffer into array starting at indx
PARAMETERS:
    a:  Pointer to the array we are  writing to
    indx:   Starting index 
    buffer: Array that holds elems we are writing into the array
    n:  Number of elements we are writing
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
RETURNS:
    true:   if the function completes without error
    false:  otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    array_setn(array * a, size_t indx, void ** buff, size_t n, ccds_error * e);

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
    true:   if the fucntion completes without error
    false:  otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    array_resize(array * a, size_t size, ccds_error * e);

/* 
NAME:
DESCRIPTION:
PARAMETERS:
    a: 
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
RETURNS:
    true:   if the fucntion completes without error
    false:  otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    array_shiftr_fill(array * a, size_t indx, size_t off, void ** buff, size_t len, ccds_error * e);

/* 
NAME:
DESCRIPTION:
PARAMETERS:
    a: 
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
RETURNS:
    true:   if the fucntion completes without error
    false:  otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/

bool    array_shiftl_fill(array * a, size_t indx, size_t off, void ** buff, size_t len, ccds_error * e);

/* 
NAME:
DESCRIPTION:
PARAMETERS:
    a: 
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
RETURNS:
    true:   if the fucntion completes without error
    false:  otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    array_swap(array * a, size_t indx1, size_t indx2, ccds_error * e);

/* 
NAME:
    array_foreach
DESCRIPTION:
    Loops over every element passing a pointer to the current elem to fn for user
        interaction. NOTE: if you need to manipulate the array dont use the 
        array methods as this funciton holds a write lock on a->buffer.
PARAMETERS:
    a:  Pointer to the array we are looping over
    fn: Pointer to caller's function that will be given pointers to the elements
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
ERRORS:
    CCDS_EOK: The function completed without error
*/
void    array_foreach(array * a, void (*fn)(void **), ccds_error * e);

/* 
NAME:
    array_foreachi
DESCRIPTION:
    Loops over every element passing a pointer to the current element, 
        and index to fn for user interaction. NOTE: if you need to manipulate 
        the array dont use the array methods as this funciton holds a write lock on a->buffer.
PARAMETERS:
    a:  Pointer to the array we are looping over
    fn: Pointer to caller's function that will be given pointers to the elements
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
ERRORS:
    CCDS_EOK: The function completed without error
*/
void    array_foreachi(array * a, void (*fn)(void **, size_t), ccds_error * e);

/* 
NAME:
    array_check_set
DESCRIPTION:
    Checks the given poision against val1 if cmp returns true then sets 
        a->buffer[i] to val2
PARAMETERS:
    a: 
    e:  Pointer to an error enum, if e != NULL then error is set accordingly. 
        Otherwise e is NULL, and no errors will be set 
RETURNS:
    true:   if the fucntion completes without error
    false:  otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    array_check_set(array * a, size_t indx, bool (*cmp) (void *,void *), void * val1, void * val2, ccds_error * e);
#endif
