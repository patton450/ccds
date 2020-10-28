#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "../src/list.h"
#include "../src/stack.h"

//arc4random(time(NULL));

static atomic_int thrd_id = 0;

int wait_time = 5; //in seconds

int threads = 4;

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
        stack_push(s, x, NULL);
        printf("Thread(%d) added %d\n", id , *x);
        fflush(stdout);
        usleep(*x * 1000);
    }
}

void * thread_rm_fn(void * param){
    int id = thrd_id++;
    stack * s  = (stack * )param;
    while(true) {
        void * dq = stack_pop(s, NULL);
        if(dq != NULL){
            int * x = (int *) dq;
            printf("\t\tThread(%d) retrived %d\n", id, *x);
            fflush(stdout);
            check[*x]--;
            free(x);
        }
        usleep(get_rand() * 1000);
    }
}

int main() {
    stack * s = stack_new(NULL, NULL);
   
    pthread_t wrk_thrds[threads];
    pthread_t rm_thrds[threads];
   
    for(int i = 0; i < threads; i++){
       pthread_create(&wrk_thrds[i], NULL, &thread_add_fn, (void *) s);
       pthread_create(&rm_thrds[i], NULL, &thread_rm_fn, (void *)s); 
    }
   
    usleep(wait_time * 1000000);

    for(int i = 0; i < threads; i++){
        pthread_cancel(wrk_thrds[i]);
        pthread_cancel(rm_thrds[i]);
    }

    for(int i = 0; i < threads; i++){
        pthread_join(wrk_thrds[i], NULL);
        pthread_join(rm_thrds[i], NULL);
    }
    
    for(int i = 0; i < LIM; i++){
        if(check[i] < 0){
            printf("%d removed twice\n", i);
        } if(check[i] > 0){
            printf("%d apprears %d\n", i, check[i]);
        }
    }
    printf("Done\n");
}
