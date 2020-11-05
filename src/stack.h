#ifndef CCDS_STACK_H
#define CCDS_STACK_H

#define STACK_DLEN 100  //Inital length for the list that represents the stack, will grow with input

#include "list.h"
#include "error.h"
#include "mem.h"

struct _stack {
    list *      rep;
    memcfg *    mem;
};

typedef struct _stack stack;

/*
NAME:
    stack_new
DESCRIPTION:
PARAMETERS:
RETURNS:
ERRORS:
    CCDS_EOK:   The funciton completed without error
*/
stack * stack_new(memcfg * mem, ccds_error * e);

/*
NAME:
    stack_free
DESCRIPTION:
PARAMETERS:
RETURNS:
ERRORS:
    CCDS_EOK:   The funciton completed without error
*/
void    stack_free(stack * s, ccds_error * e);

/*
NAME:
    stack_length
DESCRIPTION:
    Returns the number of elements in the stack
PARAMETERS:
RETURNS:
ERRORS:
    CCDS_EOK:   The funciton completed without error
*/
size_t  stack_length(stack * s, ccds_error * e);

/*
NAME:
    stack_peek
DESCRIPTION:
    Returns the pointer of the element onto the top of the stack
PARAMETERS:
RETURNS:
ERRORS:
    CCDS_EOK:   The funciton completed without error
*/
void *  stack_peek(stack * s, ccds_error * e);

/*
NAME:
    stack_pop
DESCRIPTION:
    Removes the pointer at the top of the stack and returns it
PARAMETERS:
RETURNS:
ERRORS:
    CCDS_EOK:   The funciton completed without error
*/
void *  stack_pop(stack * s, ccds_error * e);


/*
NAME:
    stack_push
DESCRIPTION:
    Pushes data onto the top of the stack
PARAMETERS:
RETURNS:
ERRORS:
    CCDS_EOK:   The funciton completed without error
*/
bool    stack_push(stack * s, void * data, ccds_error * e);

#endif
