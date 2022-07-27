#include <criterion/criterion.h>
#include "../src/array.h"

#define cap 10

array * try_array_new(size_t _cap, ccds_error * e){
    array * a = array_new(_cap, NULL, e);
    
    /* if we dont have OK then report it */
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array creation");

    /* If the array is NULL kill the test */
    cr_assert(NULL != a, "Could not allocate the array");
    return a;
}

Test(array, array_new) {
    ccds_error e;
    try_array_new(10, &e);
}

Test(array, array_null) {
    array_new(-1, NULL, NULL);
}

Test(array, array_set) {
    ccds_error e;
    array * a = array_new(cap, NULL, &e);

    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_new");

    for(size_t i = 0; i < cap; i++) {
        int * x = malloc(sizeof(int));
        cr_assert(NULL != x, "Could not allocate the integer %lu", i);
        *x = i;
        array_set(a, i, x, &e);
        cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_set");
    }

    for(size_t i = 0; i < cap; i++){
        int * x = (int *)a->buffer[i];
        cr_assert(NULL != x, "Recieved a NULL integer in position %lu", i);
        cr_expect( (*x) == i, "Expected %lu recived %lu", i, *x);
    }
}

Test(array, array_set_replace) {
    ccds_error e;
    array * a = array_new(cap, NULL, &e);

    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_new");

    for(size_t i = 0; i < cap; i++) {
        int * x = malloc(sizeof(int));
        cr_assert(NULL != x, "Could not allocate the integer %lu", i);
        *x = i;
        cr_expect(NULL == array_set(a, i, x, &e), "Expected NULL from inital set recived non NULL value");
        cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_set");
    }

    for(size_t i = 0; i < cap; i++){
        int * x = (int *)a->buffer[i];
        cr_assert(NULL != x, "Recieved a NULL integer in position %lu", i);
        cr_expect( (*x) == i, "Expected %lu recived %lu", i, *x);
    }

    int * x = malloc(sizeof(int));
    *x = 11;
    void * res = array_set(a, 0, (void * )x, &e);
    cr_assert(res, "Recived NULL value from array_set when expecting 0");
    int * tmp = a->buffer[0];

    cr_expect(tmp == x, "Expected pointer to match");
    cr_expect(*((int *)res) == 0, "Expected 0");
}

Test(array, array_set_ob){
    size_t off = 4;
    ccds_error e;
    array * a = array_new(cap, NULL, &e);

    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_new");

    array_set(a, off * cap, (void*)off, &e);
    cr_expect(CCDS_EINDX_OB == e, "Expected to get CCDS_EINDX_OB");
}

Test(array, array_get) { 
    ccds_error e;
    array * a = array_new(cap, NULL, &e);

    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_new");

    for(size_t i = 0; i < cap; i++) {
        int * x = malloc(sizeof(int));
        cr_assert(NULL != x, "Could not allocate the integer %lu", i);
        *x = i;
        a->buffer[i] = (void *)x;
    }

    for(size_t i = 0; i < cap; i++){
        int * x = array_get(a, i, &e);
        cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_get");
        cr_assert(NULL != x, "Recieved a NULL integer in position %lu", i);
        cr_expect( (*x) == i, "Expected %lu recived %lu", i, *x);
    }
}

Test(array, array_resize_grow) {
    size_t final = 100;
    ccds_error e;
    array * a = array_new(cap, NULL, &e);
    
    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_new");

    for(size_t i = 0; i < cap; i++) {
        int * x = malloc(sizeof(int));
        cr_assert(NULL != x, "Could not allocate the integer %lu", i);
        *x = i;
        array_set(a, i, x, &e);
        cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_set");
    }

    array_resize(a, final, &e);
    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_resize");
    
    for(size_t i = cap; i < final; i++) {
        int * x = malloc(sizeof(int));
        cr_assert(NULL != x, "Could not allocate the integer %lu", i);
        *x = i;
        array_set(a, i, x, &e);
        cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array creation");
    }

    for(size_t i = 0; i < final; i++){
        int * x = (int *)a->buffer[i];
        cr_assert(NULL != x, "Recieved a NULL integer in position %lu", i);
        cr_expect((*x) == i, "Expected %lu recived %lu", i, *x);
    }
}

Test(array, array_resize_shrink) { 
    int final = 100;
    ccds_error e;
    array * a = array_new(cap, NULL, &e);
    
    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_new");

    for(size_t i = 0; i < cap; i++) {
        int * x = malloc(sizeof(int));
        cr_assert(NULL != x, "Could not allocate the integer %lu", i);
        *x = i;
        array_set(a, i, x, &e);
        cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_set");
    }

    array_resize(a, final, &e);
    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_resize");
    
    for(size_t i = 0; i < final; i++){
        int * x = (int *)a->buffer[i];
        cr_assert(NULL != x, "Recieved a NULL integer in position %lu", i);
        cr_expect((*x) == i, "Expected %lu recived %lu", i, *x);
    }
}

Test(array, array_setn){
    int ** buff = malloc(sizeof(int *) * cap);
    ccds_error e;
    
    array * a = array_new(cap, NULL, &e);
    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_new");

    for(size_t i = 0; i < cap; i++) {
        int * x = malloc(sizeof(int));
        cr_assert(NULL != x, "Could not allocate the integer %lu", i);
        *x = i;
        buff[i] = x;
    }

    cr_expect(array_setn(a, 0, (void **)buff, cap, &e), "Failed at inserting items");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_setn");

    for(size_t i = 0; i < cap; i++){
        int * x = (int *) a->buffer[i];
        cr_expect(*x == i, "Expected %lu but recieved %lu", *x, i);
    }
}

Test(array, array_setn_oob){
    int ** buff = malloc(sizeof(int *) * cap);
    ccds_error e;
    
    array * a = array_new(cap, NULL, &e);
    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_new");

    for(size_t i = 0; i < cap; i++) {
        int * x = malloc(sizeof(int));
        cr_assert(NULL != x, "Could not allocate the integer %lu", i);
        *x = i;
        buff[i] = x;
    }

    cr_expect(!array_setn(a, 40, (void **)buff, cap, &e), "Failed at inserting items");
    cr_expect(CCDS_EINDX_OB == e, "Did not recieve CCDS_EINDX_OB from array_setn");
}


Test(array, array_setn_write_oob){
    int ** buff = malloc(sizeof(int *) * cap * 2);
    ccds_error e;
    
    array * a = array_new(cap, NULL, &e);
    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_new");

    for(size_t i = 0; i < cap*2; i++) {
        int * x = malloc(sizeof(int));
        cr_assert(NULL != x, "Could not allocate the integer %lu", i);
        *x = i;
        buff[i] = x;
    }

    cr_expect(!array_setn(a, 0, (void **)buff, cap*2, &e), "Failed at inserting items");
    cr_expect(CCDS_EINDX_OB == e, "Did not recieve CCDS_EINDX_OB from array_setn");
}
