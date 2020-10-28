#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "../src/stack.h"

//arc4random(time(NULL));

static atomic_int thrd_id = 0;

int wait_time = 5; //in seconds

int threads = 8;
#define LIM 200
atomic_int check[LIM];

int get_rand() {
    return arc4random() % LIM;
}

void * thread_add_fn (void * param) {
    int id = thrd_id++;
    stack * s = (stack * )param;
    while(true) {
        int * x = malloc(sizeof(int));
        *x = get_rand();
        check[*x]++;
        printf("thread: %d Adding %d to the queue\n", id, *x);
        fflush(stdout);
        stack_push(s, x, NULL);
        usleep(*x * 100);
    }
}

void * thread_rm_fn(void * param){
    stack * s = (stack * )param;
    while(true) {
        void * sp = stack_pop(s, NULL);
        if(sp != NULL){
            int * x = (int *) sp;
            printf("\t\tRetrived: %d\n", *x);
            fflush(stdout);
            check[*x]--;
            free(x);
        } else {
            usleep(100);
        }
    }
}

int main() {
    stack * s = stack_new(NULL, NULL);
   
    pthread_t wrk_thrds[threads];
    pthread_t rm_thrd;
   
    for(int i = 0; i < threads; i++){
        pthread_create(&wrk_thrds[i], NULL, &thread_add_fn, (void *) s);
    }
    pthread_create(&rm_thrd, NULL, &thread_rm_fn, (void *)s);

    usleep(wait_time * 1000000);

    for(int i = 0; i < threads; i++){
        pthread_cancel(wrk_thrds[i]);
    }
    pthread_cancel(rm_thrd);

    for(int i = 0; i < threads; i++){
        pthread_join(wrk_thrds[i], NULL);
    }
    pthread_join(rm_thrd, NULL);
    
    printf("stack length: %lu\n", stack_length(s, NULL));
    for(int i = 0; i < LIM; i++){
        if(check[i] < 0){
            printf("%d removed more that onece\n", i);
        } else if(check[i] > 0) {
            printf("%d appears %d times\n", i, check[i]);
        }
    }
    printf("Done\n");
}
