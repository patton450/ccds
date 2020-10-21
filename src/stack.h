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

stack * stack_new(memcfg * mem, ccds_error * e);
void    stack_free(stack * s, ccds_error * e);
size_t  stack_length(stack * s, ccds_error * e);

void *  stack_pop(stack * s, ccds_error * e);
void *  stack_peek(stack * s, ccds_error * e);
bool    stack_push(stack * s, void * data, ccds_error * e);

#endif
