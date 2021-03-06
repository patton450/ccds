#include "../src/array.h"
#include <criterion/criterion.h>

Test(array, array_new) {
    ccds_error e; 
    array * a = array_new(10, NULL, &e);
    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array creation");
}

Test(array, array_set) {
    size_t cap = 10;
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

Test(array, array_set_ob){
    size_t cap = 10;
    size_t off = 4;
    ccds_error e;
    array * a = array_new(cap, NULL, &e);

    cr_assert(NULL != a, "Could not allocate the array");
    cr_expect(CCDS_EOK == e, "Did not recieve CCDS_EOK from array_new");

    array_set(a, off * cap, (void*)off, &e);
    cr_expect(CCDS_EINDX_OB == e, "Expected to get CCDS_EINDX_OB");
}

Test(array, array_get) { 
    size_t cap = 10;
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
    size_t cap = 10;
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
    size_t cap = 100;
    size_t final = 10;
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
    size_t cap = 10;
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
