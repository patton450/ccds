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

int main () {
    int cap  = 100;
    list * l = list_new(cap, NULL, NULL);
    list_foreachi(l, &set_elem, NULL);

    void * buffer[l->length];
    list_map(l, buffer, cap, &map_elem, NULL);

    for(int i = 0; i < l->length; i++){
        int * x = buffer[i];
        printf("expected: %d, got: %d\n", i, *x);
        free(x);
    }

}
