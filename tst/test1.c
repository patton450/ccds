#include "../src/array.h"
#include "../src/list.h"
#include "../src/log.h"


#include <stdio.h>
#include <stdlib.h>

int main() {
    size_t cap = 100;
    list * l = list_new(cap, NULL, NULL);
    for(size_t i = 0; i < 4 * cap; i++) {
       int * ptr = malloc(sizeof(int));
       if(ptr == NULL){
            log_fatal("FUCK failed to malloc ptr");
            exit(1);
       }
       *ptr = i;
    
       if(!list_add_tail(l, ptr, NULL)){
            log_error("Failed to add %4lu, list length: %4lu", i, l->length);
       }
    }

    for(size_t i = 0; i < 4 * cap; i++){
        void * p = list_remove_head(l, NULL);
        if(p == NULL){
            log_error("Failed to retreive %lu", i);
            continue;
        }
        int x = *((int *)p);

        if(x != i) {
            log_error("list_remove_head: expected %lu, got %lu", i, x);
        }
        free(p);
    }

    list_free(l, NULL);
}
