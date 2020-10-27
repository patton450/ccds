#include <stdio.h>
#include <stdlib.h>

#include "../src/list.h"

void set_elem(void ** elem, size_t indx) {
    int * x = malloc(sizeof(int));
    *x = indx;
    *elem = x;
    log_trace("%p => %lu", x, *((int *)x));
}

void print_elem(void ** e){
    log_trace("%p => %lu", *e, *((int *) *e));
}

void add_elems(void * a, void * b){
    log_trace("_a = %p, _b = %p", a, b);
    int * _a = a;
    int * _b = (int *)b;

    *_a = (*_a + *_b);
}

int main () {
    int cap  = 101;
    list * l = list_new(cap, NULL, NULL);
    list_foreachi(l, &set_elem, NULL);
    
    printf("\n");
    list_foreach(l, &print_elem, NULL);

    int acc= 0;
    list_foldl(l, &acc, &add_elems, NULL);

    log_info("%d", acc);
}
