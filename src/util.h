#ifndef CCDS_UTIL_H
#define CCDS_UTIL_H

#include "log.h"

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

#endif
