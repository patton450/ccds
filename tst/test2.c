#include <stdio.h>
#include <stdlib.h>

#include "../src/array.h"
#include "../src/list.h"

void print_int_ptr(void ** ptr, size_t indx){
    if(ptr == NULL) { return; }
    if(*ptr == NULL) { return; }
    int * x = (int *)(*ptr);
    log_info("%p => a->buffer[%lu] = %d", x, indx, *x);
};

int main(){
    int cap = 100;
    list * l = list_new(cap, NULL, NULL);

    cap *= 4;
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
   
    list_foreachi(l, &print_int_ptr, NULL);


    printf("\n");
    log_info("Done verifying\n");

    int removeIndx = 0;
    for(int i = 0; i < cap - removeIndx; i++) {
        void * ptr = list_remove(l, removeIndx, NULL);
        if(ptr != NULL) {
            log_info("%p = a->buffer[%d] := %d", ptr, removeIndx, *((int *)ptr));
        } else {
            log_info("NULL = a->buffer[%d]", removeIndx);
        }
        free(ptr);
    }
    
    list_free(l, NULL);
}
