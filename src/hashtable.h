#ifndef CCDS_HASHTABLE_H
#define CCDS_HASHTABLE_H

#include "array.h"
#include "list.h"
#include "mem.h"
#include "error.h"
#include "log.h"
#include "util.h"

typedef uint64_t    (*hash)(void *)         uhash64_fn;
typedef bool        (*cmp)(void *, void *)  kcmp_fn;

struct _key_val{
    void * key;
    void * val;
}

typedef struct _key_val key_val;

struct _hashtable {
    uhash64_fn                  hash;
    kcmp_fn                     kcmp;
    array *                     rep;        //Array of lists
    atomic_size_t               length;     //Total number of Key-Val Pairs, can be >= rep->cap
    memcfg *                    mem;
};

typedef struct _hashtable hashtable;

hashatbale *    hashatable_new(size_t cap, uhash64_fn h, kcmp_fn kcmp, memcfg * mem, ccds_error * e);
void            hashtable_free(hashtable * ht, ccds_error * e);
size_t          hashtable_length(hashtable * ht, ccds_error * e);


bool hashtable_add(hashtable * ht, void * key, void * val, ccds_error * e);
void * hashtable_get(hashtable * ht, void * key, ccds_error * e);
void * hashtable_remove(hashatable * ht, void * key, ccds_error * e);

void hashtable_keys(hashtable * h, void ** keys_out, size_t ko_len, ccds_error * e);
#endif
