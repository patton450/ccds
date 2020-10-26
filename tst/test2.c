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

void print_int_ptr(void ** ptr){
    if(ptr == NULL) { return; }
    if(*ptr == NULL) { return; }
    int * x = (int *)(*ptr);
    printf("%p => %d \n", x, *x);
    free(x);
    *ptr = NULL;
};

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
   
    list_foreach(l, &print_int_ptr, NULL);
//    for(int i = 0; i < cap; i++){
//        void * ptr = list_get(l, i, NULL);
//        log_info("%p = a->buffer[%d] := %d", ptr, i, *((int *)ptr));
//    }


    printf("\n");
    log_info("Done verifying\n");

    for(int i = 0; i < cap -1; i++) {
        void * ptr = list_remove(l, 1, NULL);
        size_t indx = 1;
        if(ptr != NULL) {
            log_info("%p = a->buffer[%d] := %d", ptr, indx, *((int *)ptr));
        } else {
            log_info("NULL = a->buffer[%d]", indx);
        }
        free(ptr);
    }
    
    list_free(l, NULL);
}
