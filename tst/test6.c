#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../src/list.h"

const int cap = 1000;
const int threads = 8;

void print_int(void ** i){
    int * x = (int * ) *i;
    printf("%d\n", *x);
}

void * thread_fn(void * ls){
    list * l = (list *)ls;
    /*  each thread adds [0, cap-1] so after all threads complete 
        we should have threads * each of those ints*/
    for(int i = 0; i < cap; i ++) {
        int * x = malloc(sizeof(int));
        if(!x){ fprintf(stderr, "failed to allocate int pointer \n"); return NULL; }
        *x = i;
//        log_info("%lu", l->length);
        if(!list_add(l, i, x, NULL)){
            fprintf(stderr, "Could not add elem: %d", i);
        }
    }
    return NULL;
}

int main() {
    list * test_list = list_new(cap, NULL, NULL);
    
    int check_arr[cap];
    for(int i = 0; i < cap; i++){
        check_arr[i] = threads;    
    }

    pthread_t thrds[threads];
    for(int i = 0; i < threads; i++){
        pthread_create(&thrds[i], NULL, &thread_fn, (void *)test_list); 
    }

    for(int i = 0; i < threads; i++) {
        pthread_join(thrds[i], NULL); 
    }
    

    for(int i = 0; i < threads * cap; i++){
        int * x = (int *)list_remove_head(test_list, NULL);
        if(x == NULL){ fprintf(stderr, "x is NULL"); return 1;}
        check_arr[*x]--;
        free(x);
    }
    
    for(int i = 0; i < cap; i++){
        if(check_arr[i] != 0){
            printf("extra copy of %d", i);
        }
    }

    printf("Done!\n");
}
