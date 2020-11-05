#ifndef CCDS_MEM_H
#define CCDS_MEM_H

#include <stdlib.h>
#include <string.h>

/* Macros for the default memory management funcitons */
#define ccds_dfree      free
#define ccds_dmalloc    malloc
#define ccds_drealloc   realloc
#define ccds_dcalloc    calloc

/* Struct for handeling user defined memory mngmt */
struct _memcfg {
    void (*free)(void *);
    void * (*malloc)(size_t);
    void * (*calloc)(size_t, size_t);
    void * (*realloc)(void *, size_t);
};

typedef struct _memcfg memcfg;


/* Memory maniputalion methods, if m is NULL, or the specified method in m is NULL
    then the default method is used */
#define memcfg_free(m, ptr)             !(m && m->free) ? ccds_dfree(ptr) : m->free(ptr)
#define memcfg_malloc(m, size)          !(m && m->malloc) ? ccds_dmalloc(size) : m->malloc(size)
#define memcfg_calloc(m, cnt, size)     !(m && m->calloc) ? ccds_dcalloc(cnt, size) : m->calloc(cnt, size)
#define memcfg_realloc(m, ptr, size)    !(m && m->realloc) ? ccds_drealloc(ptr, size) : m->realloc(ptr, size)

#endif
