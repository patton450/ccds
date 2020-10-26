#include "../src/array.h"
#include "../src/list.h"
#include "../src/log.h"


#include <stdio.h>
#include <stdlib.h>

int main() {
    size_t cap = 100;
    list * l = list_new(cap, NULL, NULL);

    log_debug("array buffer: %p\n", l->buffer->buffer);
    for(size_t i = 0; i < cap; i++) {
       int * ptr = malloc(sizeof(int));
       
       if(ptr == NULL){
            log_fatal("FUCK failed to malloc ptr");
            exit(1);
       }
       
       *ptr = i;
    
       if(!list_add_tail(l, ptr, NULL)){
            log_error("Failed to add %4lu, list length: %4lu", i, l->length);
       }

       log_trace("%p => arr[i]:= %p", ptr, l->buffer->buffer[i]);
    }
    
    printf("\n");
    log_debug("array buffer: %p\n", l->buffer->buffer);
    for(size_t i = 0; i < cap; i++){
        void * ptr = list_get(l, i, NULL);
        log_info("%p => %p => %d", &(l->buffer->buffer[i]), ptr, i);
    }
    
    printf("\n");

    for(size_t i = 0; i < cap; i++){
        log_debug("array buffer: %p", l->buffer->buffer);
        void * p = list_remove_head(l, NULL);
        
        if(p == NULL){
            log_error("Failed to retreive %lu", i);
            continue;
        }
        
        int *x = ((int *)p);
        log_trace("Got: %p at %lu", x, i);
        if(*x != i) {
            log_error("list_remove_head: expected %lu, got %lu", i, *x);
        }

        //free(p);
        printf("\n");
    }

    list_free(l, NULL);
}
