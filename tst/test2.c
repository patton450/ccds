#include <stdio.h>
#include <stdlib.h>

#include "../src/array.h"
#include "../src/list.h"

int mawin(){
    int cap = 100;
    array * a = array_new(cap, NULL, NULL);
    
    for(int i = 0; i < cap - 1; i++) {
        int * x = malloc(sizeof(int));
        *x = i;
        
        void * tmp[1];
        tmp[0] = x;

        if(array_insert_shift(a, i, 1, tmp, NULL)) {
            log_info("%p = a->buffer[%d] := %d", x, i, i);
        } else {
            log_error("Failed to inser %p at %d", x, i);
        }
    }

    printf("\n");
    log_info("Done loading\n");
    for(int i = 0; i < cap -1; i++){
        void * ptr = array_get(a, i, NULL);
        log_info("%p = a->buffer[%d] := %d", ptr, i, *((int *)ptr));
    }
    
    printf("\n");
    log_info("Done verifying\n");
    return 0;
}

int main(){
    int cap = 100;
    list * l = list_new(cap, NULL, NULL);

    for(int i = 0; i < cap; i++){
        int * x = malloc(sizeof(int));
        *x = i;
        if(list_add_tail(l, x, NULL)){
            log_info("%p = a->buffer[%d] := %d", x, i, i);
        } else {
            log_error("Failed to inser %p at %d", x, i);
        }
    }

    printf("\n");
    log_info("Done loading\n");
    
    for(int i = 0; i < cap; i++){
        void * ptr = list_get(l, i, NULL);
        log_info("%p = a->buffer[%d] := %d", ptr, i, *((int *)ptr));
    }


    printf("\n");
    log_info("Done verifying\n");

    for(int i = 0; i < cap -1; i++) {
        void * ptr = list_remove(l, 1, NULL);
        log_info("%p = a->buffer[%d] := %d", ptr, 1, *((int *)ptr));
        free(ptr);
    }
    
    list_free(l, NULL);
}
