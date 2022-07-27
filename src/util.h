#ifndef CCDS_UTIL_H
#define CCDS_UTIL_H

#include "log.h"
#include "error.h"

#define MAX(a, b) ({         \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b;      \
})


#define MIN(a,b) ({         \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _b : _a;      \
})

#define EXIST(a,b,c) ({                                                               \
    if(a == NULL) {                                                                 \
        log_error("NULL %s passed into %s", c, __FUNCTION__);     \
        CCDS_SET_ERR(b, CCDS_EINVLD_PARAM);                                        \
    }                                                                               \
})

#endif
