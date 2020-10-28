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
    ccds_mtx        expand;
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
    CCDS_EOK: The funciton completed without error
*/
void    list_free(list * l, ccds_error * e);


/* 
NAME:
    list_length
DESCRIPTION:
    Rteurns the length of the list
PARAMETERS:
    l:      List we want to get the length of
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    size_t: Length of the list
ERRORS:
    CCDS_EOK: The funciton completed without error
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
    true: If the funciton completed without error
    false: Otherwise
ERRORS:
    CCDS_EOK: The funciton completed without error
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
    true: If the funciton completed without error
    false: Otherwise
ERRORS:
    CCDS_EOK: The funciton completed without error
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
    true: If the funciton completed without error
    false: Otherwise
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
bool    list_add_tail(list * l, void * data, ccds_error * e);


/* 
NAME:
    list_get
DESCRIPTION:
    Returns the element at index
PARAMETERS:
    l:      List we areoperating on
    indx:   Index we want to access
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: The element at index
    NULL:   If the funciton encuntered an error
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
    void *: The element at index
    NULL:   If the funciton encuntered an error
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
    void *: The element at index
    NULL:   If the funciton encuntered an error
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

/* 
NAME:
    list_swap
DESCRIPTION:
    Swaps elements at the given indices
PARAMETERS:
    l:      The list we are operating on
    indx1:  The index the element at index2 will end up at
    indx2:  The index the element at index1 will end up at
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true: If the funciton completed without error
    false: Otherwise
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
bool    list_swap(list * l, size_t indx1, size_t indx2, ccds_error * e);

/* 
NAME:
    list_foreach
DESCRIPTION:
    Loops through all of the elements in list passing their pointers into fn
PARAMETERS:
    l:      The list we are looping over
    fn:     The function we are passing a pointer to an element to
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void    list_foreach(list * l, void (*fn)(void**), ccds_error * e);


/* 
NAME:
    list_foreachi
DESCRIPTION:
    Loops through all of the elements in list passing their pointers, and indices into fn
PARAMETERS:
    l:      The list we are looping over
    fn:     The function we are passing an element's pointer, and indice to
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RRORS:
    CCDS_EOK: The funciton completed without error
*/
void    list_foreachi(list * l, void (*fn)(void**, size_t), ccds_error * e);

/* 
NAME:
    list_foldl
DESCRIPTION:
    Applies the funciton to every element in a lef associative manner,
        ie. if we pass a function f, and a list containing [1,2,3,..,n]
            with a start value of 0 we get  f( ... f(f(f(0,1), 2), 3), ... n)
PARAMETERS:
    l:      List we are operating on
    void *: Starting value that will occupy the left argument of the first function call
    fn:     Function we are applying to every element     
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: A pointer to the accumulated value if fn applied to lists's elements
    NULL:   If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void *  list_foldl(list * l, void * start, void (*fn) (void *, void *), ccds_error * e);


/* 
NAME:
    list_foldr
DESCRIPTION:
    Applies the funciton to every element in a right associative manner,
        ie. if we pass a function f, and a list containing [1,2,3,..,n]
            with a start value of 0 we get  f(1, ... f(n-2,f(n-1,f(n, 0))) ... )
PARAMETERS:
    l:      List we are operating on
    void *: Starting value that will occupy the right argument of the first function call
    fn:     Function we are applying to every element     
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    void *: A pointer to the accumulated value if fn applied to lists's elements
    NULL:   If the funciton encuntered an error
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void *  list_foldr(list * l, void * start, void (*fn) (void *, void *), ccds_error * e);

/* 
NAME:
    list_map
DESCRIPTION:
    Applies fn to every element in l, or until buff_len is met. Passes en element of the list,
        and a pointer to an element of buffer. The void * you want to save should be placed as the value
        of the second argument
PARAMETERS:
    l:          List we are operating on
    buff:       Array of void pointer that is used for output
    buff_len:   Maxmimum length of buffer
    fn:         Function we are using to test elements in the list
    e:          Pointer to an error enum, if e != NULL then error is set accordingly. 
                    Otherwise e is NULL, and no errors will be set.
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void    list_map (list * l, void ** buff, size_t buff_len, void (*fn) (void *, void **), ccds_error * e);


/* 
NAME:
    list_filter
DESCRIPTION:
    Applies fn to every element in l, or until buff_len is met. Passes en element of the list to fn
        if fn returns true then that element will be placed into buff
PARAMETERS:
    l:          List we are operating on
    buff:       Output buffer where the elements that pass fn are put into
    buff_len:   The maxmimum length of the buffer
    fn:         Function we are using to test elements in the list
    e:          Pointer to an error enum, if e != NULL then error is set accordingly. 
                    Otherwise e is NULL, and no errors will be set.
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
void    list_filter(list * l, void ** buff, size_t buff_len, bool (*fn) (void *), ccds_error * e);


/* 
NAME:
    list_any
DESCRIPTION:
    Checks if at least one element in the list passes the function supplied.
PARAMETERS:
    l:      List we are operating on
    fn:     Function we are using to test elements in the list
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true: if fn returns true for at least one element in l
    false: otherwise
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
bool    list_any(list * l, bool (*fn) (void *), ccds_error * e);


/* 
NAME:
    list_all
DESCRIPTION:
    Checks if every element in the list passes the function supplied.
PARAMETERS:
    l:      List we are operating on
    fn:     Function we are using to test elements in the list
    e:      Pointer to an error enum, if e != NULL then error is set accordingly. 
                Otherwise e is NULL, and no errors will be set.
RETURNS:
    true: If fn returns true for every element in l
    false: Otherwise
ERRORS:
    CCDS_EOK: The funciton completed without error
*/
bool    list_all(list * l, bool (*fn) (void *), ccds_error * e);
#endif
