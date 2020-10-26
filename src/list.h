#ifndef CCDS_LIST_H
#define CCDS_LIST_H

#include <stdatomic.h>

#include "mem.h"
#include "array.h"
#include "lock.h"
#include "error.h"
#include "util.h"
#include "log.h"

struct _list {
    atomic_size_t   length;
    array *         buffer;
    memcfg *        mem;
    ccds_mtx *      expand;
};

typedef struct _list list;

/* 
NAME:
    list_new
DESCRIPTION:
    Allocates space for a new list and returns the pointer
PARAMETERS:
    cap:    Inital capacity for the list
    mem:    Memory config struct for user memory mangment or NULL for defaults
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    NULL: If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/

list *  list_new(size_t cap, memcfg * mem, ccds_error * e);
void    list_free(list * l, ccds_error * e);
size_t  list_length(list * l, ccds_error * e);

bool    list_add(list * l, size_t indx, void * data, ccds_error * e);
bool    list_add_head(list * l, void * data, ccds_error * e);
bool    list_add_tail(list * l, void * data, ccds_error * e);

/* 
NAME:
    list_get
DESCRIPTION:
PARAMETERS:
    l:      List we areoperating on
    ind:    Index we want to access
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    NULL: If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void *  list_get(list * l, size_t indx, ccds_error * e);

/* 
NAME:
    list_get_head
DESCRIPTION:
    Returns the first element in the list
PARAMETERS:
    l:      List we areoperating on
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    NULL: If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void *  list_get_head(list * l, ccds_error * e);

/* 
NAME:
    list_get_tail
DESCRIPTION:
    Returns the last element of the list
PARAMETERS:
    l:      List we areoperating on
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    NULL: If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void *  list_get_tail(list * l, ccds_error * e);

/* 
NAME:
    list_remove
DESCRIPTION:
    Removes the element at indx
PARAMETERS:
    l:      List we areoperating on
    indx:   Index we want to remove
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The element that occupied the list at index before the funciton call
    NULL:   If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void *  list_remove(list * l, size_t indx, ccds_error * e);


/* 
NAME:
    list_remove_head
DESCRIPTION:
    Removes the first element of the list
PARAMETERS:
    l:      List we areoperating on
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The fist element of the list 
    NULL:   If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void *  list_remove_head(list * l, ccds_error * e);


/* 
NAME:
    list_remove_tail
DESCRIPTION:
    Removes the last element of the list
PARAMETERS:
    l:      List we areoperating on
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The last element of the list
    NULL:   If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void *  list_remove_tail(list * l, ccds_error * e);

bool    list_swap(list * l, size_t indx1, size_t indx2, ccds_error * e);

void    list_foreach(list * l, void (*fn)(void**), ccds_error * e);
void    list_foreachi(list * l, void (*fn)(void**, size_t), ccds_error * e);

/* 
NAME:
    list_foldl
DESCRIPTION:
PARAMETERS:
    l:      List we areoperating on
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    NULL: If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void *  list_foldl(list * l, void * start, void * (*fn) (void *, const void *), ccds_error * e);


/* 
NAME:
    list_foldr
DESCRIPTION:
PARAMETERS:
    l:      List we areoperating on
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    NULL: If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void *  list_foldr(list * l, void * start, void * (*fn) (const void *, void *), ccds_error * e);

void    list_map (list * l, void ** buff, size_t buff_len, void * (*fn) (void *), ccds_error * e);
void    list_filter(list * l, void ** buff, size_t buff_len, bool (*fn) (void *), ccds_error * e);

bool    list_any(list * l, bool (*fn) (void *), ccds_error * e);
bool    list_all(list * l, bool (*fn) (void *), ccds_error * e);
#endif
