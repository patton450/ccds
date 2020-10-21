#include "stack.h"

stack * stack_new(memcfg * m, ccds_error * e){
    stack * s = memcfg_malloc(m, sizeof(stack));
    if(s == NULL){
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }

    s->rep = list_new(STACK_DLEN, m, e);
    if(s->rep == NULL){
        memcfg_free(m, s);
    }
    
    s->mem = m;
    return s;
}

void stack_free(stack * s, ccds_error * e){
    if(s == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return;
    }
    
    list_free(s->rep, e);
    memcfg_free(s->mem, s);
}

size_t stack_length(stack * s, ccds_error * e) {
    if(s == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return 0;
    }
    return list_length(s->rep, e);
}

void * stack_peek(stack * s, ccds_error * e){
    if(s == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }
    return list_get_tail(s->rep, e);
}

void * stack_pop(stack * s, ccds_error * e){
    if(s == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return NULL;
    }
    return list_remove_tail(s->rep, e);
}

bool stack_push(stack * s, void * data, ccds_error * e){
    if(s == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARAM);
        return false;
    }
    return list_add_tail(s->rep, data, e);
}

