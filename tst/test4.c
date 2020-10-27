#include <stdio.h>
#include <stdlib.h>

#include "../src/list.h"

void set_elem(void ** elem, size_t indx) {
    log_trace("%lu", indx);
    int * x = malloc(sizeof(int));
    *x = indx;
    *elem = x;
}

void map_elem(void ** in, void ** out){
    if(in == NULL){ return; }
    if(*in == NULL){ return; }

    int * t = malloc(sizeof(int));
    int * p = (int *) (*in);
    *t = (*p) * (*p); 
    *out = t;
}

bool filt_elem(void * elem){
    int * x = (int * )elem;
    return *x % 2 == 0;
}

int main () {
    int cap  = 100;
    list * l = list_new(cap, NULL, NULL);
    list_foreachi(l, &set_elem, NULL);

    void * buffer[l->length];
    memset(buffer, 0, l->length * sizeof(void *));
    list_filter(l, buffer, cap, &filt_elem, NULL);

    for(int i = 0; i < l->length; i++){
        int * x = buffer[i];
        if(x == NULL){ continue; }
        printf("expected: %d, got: %d\n", i, *x);
        free(x);
    }

}
