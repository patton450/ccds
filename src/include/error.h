#ifndef CCDS_ERRORS_H
#define CCDS_ERRORS_H

#include <errno.h>

enum _ccds_error {
    CCDS_EOK,               /* No errors occured */
    CCDS_EMEM_FAIL,         /* Failed to allocate memory, ie *alloc failed */
    CCDS_ERSC_FAIL,         /* Failed to create necssary resources */
    CCDS_EPERM,             /* Does not have necessary permissions */
    CCDS_EINDX_OB,          /* Index provided was out of bounds */
    CCDS_EINVLD_PARAM,      /* One or more parameters passed to a function was invalid */
    CCDS_EBUSY,             /* The resource is currently busy and cant preform the action*/
    CCDS_EPRE_COND,         /* A preconditon on the funciton was not met at runtime */
};

typedef enum _ccds_error ccds_error;

void ccds_print_error(ccds_error e) {
    switch(e){
        case CCDS_EOK:
        case CCDS_EMEM_FAIL:
        case CCDS_ERSC_FAIL:
        case CCDS_EPREM:
        case CCDS_EINDX_OB:
        case CCDS_EBUSY:
        case CCDS_E_PRE_COND:
            printf("Failed to meet a precondition");
            break;
        default:
            printf("Unrecognized error occured");
            break;
    }
}

#define CCDS_SET_ERR(e, val) ({     \
            if(e) {                 \
                *e = val;           \
            }})
#endif
