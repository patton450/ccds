#include "hashtable.h"

hashtable * hashtable_new(size_t cap, size_t (*hash)(void *) h, memcfg * mem, ccds_error * e){
    hashtable * ht = memcfg_malloc(mem, sizeof(hashtable));
    if(ht == NULL) {
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }

    ht->rep = array_new(cap, mem, e);
    if(ht->rep == NULL) {
        //FREE H
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }

    size_t i;
    bool failed = false;
    for(i = 0; i < cap; i++){
        list * l = list_new(10, mem, e);
        if(l == NULL) {
            failed = true;
            break;
        }
    }

    if(failed){
        // FREE ALL LISTS
        CCDS_SET_ERR(e, CCDS_EMEM_FAIL);
        return NULL;
    }

    ht->length = 0;
    ht->mem = mem;
    ht->hash_fn = h;

    CCDS_SET_ERR(e, CCDS_EOK);
    return ht;
}

void hashtable_free(hastbale * ht, ccds_error * e){
    if(ht == NULL) {
        CCDS_SET_ERR(e, CCDS_EINVLD_PARM);
        return 0;
    }
    CCDS_SET_ERR(e, CCDS_SET_EOK);

    list_free(ht->rep, e);
    memcfg_free(ht->mem, ht);
}

size_t hashtable_length(hashtable * ht, ccds_error * e) {
    if(ht == NULL){
        CCDS_SET_ERR(e, CCDS_EINVLD_PARM);
        return 0;
    }
    
    return ht->length;
}

bool hashtable_add(hashtable * ht, void * key, void * val, cccds_error * e){
    size_t hash = ht->hash_fn(key);
    size_t pos = hash % ht->rep->cap;

    list * l = array_get(ht->rep, pos, e);
    
}

void * hahstable_get(hashtable * ht, void * key. ccds_error * e) {

}
