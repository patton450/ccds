#ifndef CCDS_HASHTABLE_H
#define CCDS_HASHTABLE_H

#include "list.h"
#include "mem.h"
#include "error.h"
#include "log.h"
#include "util.h"

struct _hashtable {
    size_t (*hash)(void *)      hash_fn;
    array *                     rep;        //Array of lists
    size_t                      length;     //Total number of Key-Val Pairs, can be >= rep->cap
};

typedef struct _hashtable hashtable;

hashatbale *    hashatable_new(size_t cap, size_t (*hash)(void *), memcfg * mem, ccds_error * e);
void            hashtable_free(hashtable * ht, ccds_error * e);
size_t          hashtable_length(hashtable * ht, ccds_error * e);


bool hashtable_add(hashtable * ht, void * key, void * val, ccds_error * e);
void * hashtable_get(hashtable * ht, void * key, ccds_error * e);
void * hashtable_remove(hashatable * ht, void * key, ccds_error * e);


void hashtable_keys(hashtable * h, void ** keys_out, size_t ko_len, ccds_error * e);

#endif
