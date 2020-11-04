#ifndef CCDS_HASHTABLE_H
#define CCDS_HASHTABLE_H

struct _hashtable {
    size_t (*hash)(void *)      hash_fn;
};

typedef struct _hashtable hashtable;

hashatbale *    hashatable_new(size_t cap, size_t (*hash)(void *), memcfg * mem, ccds_error * e);
void            hashtable_free(hashtable * ht, ccds_error * e);
size_t          hashtable_length(hashtable * ht, ccds_error * e);


bool hashtable_try_add(hashtable * ht, void * key, void * val, ccds_error * e);
void * hashtable_try_get(hashtable * ht, void * key, ccds_error * e);
void * hashtable_try_remove(hashatable * ht, void * key, ccds_error * e);


void hashtable_keys ()

#endif
