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
    atomic_size_t   length;         /* Number of elements in the list*/
    array *         buffer;         /* Array that will represent the list*/
    memcfg *        mem;            /* User defined memory allocation */
    ccds_mtx        expand;         /* Ensures only one thread can expand the array at a time*/
};

typedef struct _list list;


/* 
NAME:
    list_new
DESCRIPTION:
    Allocates space for a new list and returns the pointer
PARAMETERS:
    cap:    Initial capacity for the list
    mem:    memcfg struct * for user memory allocation or NULL for default memory allocators
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    NULL: If the function encountered an error
ERRORS:
    CCDS_EOK: The function completed without error
*/

list *  list_new(size_t cap, memcfg * mem, ccds_error * e);


/* 
NAME:
    list_free
DESCRIPTION:
    Frees space allocated to list in list_free
PARAMETERS:
    l:      List we are freeing
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
ERRORS:
    CCDS_EOK: The function completed without error
*/
void    list_free(list * l, ccds_error * e);


/* 
NAME:
    list_length
DESCRIPTION:
    Returns the length of the list
PARAMETERS:
    l:      List we want to get the length of
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    size_t: Length of the list
ERRORS:
    CCDS_EOK: The function completed without error
*/
size_t  list_length(list * l, ccds_error * e);


/* 
NAME:
    list_add
DESCRIPTION:
    Adds data at index of the list
PARAMETERS:
    l:      List we are adding to
    indx:   Index we want to add data at
    data:   Element we are adding to the list
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true: If the function completed without error
    false: Otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    list_add(list * l, size_t indx, void * data, ccds_error * e);


/* 
NAME:
    list_add_head
DESCRIPTION:
    Adds element to the front of the list
PARAMETERS:
    l:      List we are adding to
    data:   Element we are adding to the list
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true: If the function completed without error
    false: Otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    list_add_head(list * l, void * data, ccds_error * e);


/* 
NAME:
    list_add_tail
DESCRIPTION:
    Adds element at the end of the list
PARAMETERS: 
    l:      List we are adding to
    data:   Element we are adding to the list
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true: If the function completed without error
    false: Otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    list_add_tail(list * l, void * data, ccds_error * e);


/* 
NAME:
    list_get
DESCRIPTION:
    Returns the element at index
PARAMETERS:
    l:      The list we are reading
    indx:   Index we want to access
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The element at index
    NULL:   If the function encountered an error
ERRORS:
    CCDS_EOK: The function completed without error
*/
void *  list_get(list * l, size_t indx, ccds_error * e);

/* 
NAME:
    list_get_head
DESCRIPTION:
    Returns the first element in the list
PARAMETERS:
    l:      The list we are reading
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The element at index
    NULL:   If the function encountered an error
ERRORS:
    CCDS_EOK: The function completed without error
*/
void *  list_get_head(list * l, ccds_error * e);

/* 
NAME:
    list_get_tail
DESCRIPTION:
    Returns the last element of the list
PARAMETERS:
    l:      The list we are reading
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The element at index
    NULL:   If the function encountered an error
ERRORS:
    CCDS_EOK: The function completed without error
*/
void *  list_get_tail(list * l, ccds_error * e);

/* 
NAME:
    list_remove
DESCRIPTION:
    Removes the element at index
PARAMETERS:
    l:      The list we are editing
    indx:   Index we want to remove
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The element that occupied the list at index before the function call
    NULL:   If the function encountered an error
ERRORS:
    CCDS_EOK: The function completed without error
*/
void *  list_remove(list * l, size_t indx, ccds_error * e);


/* 
NAME:
    list_remove_head
DESCRIPTION:
    Removes the first element of the list
PARAMETERS:
    l:      The list we are editing
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The first element of the list 
    NULL:   If the function encountered an error
ERRORS:
    CCDS_EOK: The function completed without error
*/
void *  list_remove_head(list * l, ccds_error * e);


/* 
NAME:
    list_remove_tail
DESCRIPTION:
    Removes the last element of the list
PARAMETERS:
    l:      The list we are editing
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The last element of the list
    NULL:   If the function encountered an error
ERRORS:
    CCDS_EOK: The function completed without error
*/
void *  list_remove_tail(list * l, ccds_error * e);

/* 
NAME:
    list_swap
DESCRIPTION:
    Swaps elements at the given indices
PARAMETERS:
    l:      The list we are editing
    indx1:  The index the element at index2 will end up at
    indx2:  The index the element at index1 will end up at
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true: If the function completed without error
    false: Otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    list_swap(list * l, size_t indx1, size_t indx2, ccds_error * e);

/* 
NAME:
    list_foreach
DESCRIPTION:
    Loops through all of the elements in list passing their pointers into function
PARAMETERS:
    l:      The list we are looping over
    fn:     The function we are passing a pointer to an element to
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
ERRORS:
    CCDS_EOK: The function completed without error
*/
void    list_foreach(list * l, void (*fn)(void**), ccds_error * e);


/* 
NAME:
    list_foreachi
DESCRIPTION:
    Loops through all of the elements in list passing their pointers, and indices into function
PARAMETERS:
    l:      The list we are looping over
    fn:     The function we are passing an element's pointer and indice to
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
ERRORS:
    CCDS_EOK: The function completed without error
*/
void    list_foreachi(list * l, void (*fn)(void**, size_t), ccds_error * e);

/* 
NAME:
    list_foldl
DESCRIPTION:
    Applies the function to every element in a left associative manner,
        i.e. if we pass a function f, and a list containing [1,2,3,..,n]
            with a start value of 0 we get  f( ... f(f(f(0,1), 2), 3), ... n)
PARAMETERS:
    l:      List we are operating on
    void *: Starting value that will occupy the left argument of the first function call
    fn:     Function we are applying to every element     
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The pointer to the accumulated value of appling the function 
    NULL:   If the function encountered an error
ERRORS:
    CCDS_EOK: The function completed without error
*/
void *  list_foldl(list * l, void * start, void (*fn) (void *, void *), ccds_error * e);


/* 
NAME:
    list_foldr
DESCRIPTION:
    Applies the function to every element in a right associative manner,
        i.e. if we pass a function f, and a list containing [1,2,3,..,n]
            with a start value of 0 we get  f(1, ... f(n-2,f(n-1,f(n, 0))) ... )
PARAMETERS:
    l:      List we are operating on
    void *: Starting value that will occupy the right argument of the first function call
    fn:     Function we are applying to every element     
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The pointer to the accumulated value of appling the function 
    NULL:   If the function encountered an error
ERRORS:
    CCDS_EOK: The function completed without error
*/
void *  list_foldr(list * l, void * start, void (*fn) (void *, void *), ccds_error * e);

/* 
NAME:
    list_map
DESCRIPTION:
    Applies the function to every element in the list, or until the buffer's length is met. 
        Passes an element of the list, and a pointer to its output location in buffer. 
        The void * you want to save should be placed as the value of the second argument
PARAMETERS:
    l:          List we are operating on
    buff:       Array of void pointer that is used for output
    buff_len:   Maxmimum length of buffer
    fn:         Function we are using to test elements in the list
    e:          Pointer to an error enum, if e != NULL then error is set accordingly. 
                    Otherwise e is NULL, and no errors will be set.
ERRORS:
    CCDS_EOK: The function completed without error
*/
void    list_map (list * l, void ** buff, size_t buff_len, void (*fn) (void *, void **), ccds_error * e);


/* 
NAME:
    list_filter
DESCRIPTION:
    Applies the function to every element in the list. If the function returns true for a given element then
        the element is added to buffer until the buffer is full.
PARAMETERS:
    l:          List we are operating on
    buff:       Output buffer that holds the elements that pass fn
    buff_len:   The maxmimum length of the buffer
    fn:         Function we are using to test elements in the list
    e:          Pointer to an error enum, if e != NULL then error is set accordingly. 
                    Otherwise e is NULL, and no errors will be set.
ERRORS:
    CCDS_EOK: The function completed without error
*/
void    list_filter(list * l, void ** buff, size_t buff_len, bool (*fn) (void *), ccds_error * e);


/* 
NAME:
    list_any
DESCRIPTION:
    Checks if at least one element in the list passes the given function.
PARAMETERS:
    l:      List we are operating on
    fn:     Function we are using to test elements in the list
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true: if the function returns true for at least one element in the list
    false: otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    list_any(list * l, bool (*fn) (void *), ccds_error * e);


/* 
NAME:
    list_all
DESCRIPTION:
    Checks if every element in the list passes the given function.
PARAMETERS:
    l:      List we are operating on
    fn:     Function we are using to test elements in the list
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true: If the function returns true for every element in the list
    false: Otherwise
ERRORS:
    CCDS_EOK: The function completed without error
*/
bool    list_all(list * l, bool (*fn) (void *), ccds_error * e);
#endif
