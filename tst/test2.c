#include <stdio.h>
#include <stdlib.h>

#include "../src/array.h"
#include "../src/list.h"

void print_int_ptr(void ** ptr){
    if(ptr == NULL) { return; }
    if(*ptr == NULL) { return; }
    int * x = (int *)(*ptr);
    printf("%p => %d \n", x, *x);
//    free(x);
//    *ptr = NULL;
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
