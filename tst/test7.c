#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "../src/list.h"
#include "../src/queue.h"

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
    queue * q = (queue * )param;
    while(true) {
        int * x = malloc(sizeof(int));
        *x = get_rand();
        //check[*x]++;
        printf("thread: %d Adding %d to the queue\n", id, *x);
        fflush(stdout);
        queue_enqueue(q, x, NULL);
        usleep(*x);
    }
}

void * thread_rm_fn(void * param){
    queue * q = (queue * )param;
    while(true) {
        void * dq = queue_dequeue(q, NULL);
        if(dq != NULL){
            int * x = (int *) dq;
            printf("\tRetrived: %d\n", *x);
            fflush(stdout);
            //check[*x]--;
            //free(x);
        } else {
            usleep(10);
        }
    }
}

int main() {
    queue * q = queue_new(NULL, NULL);
   
    pthread_t wrk_thrds[threads];
    pthread_t rm_thrd;
   
    for(int i = 0; i < threads; i++){
        pthread_create(&wrk_thrds[i], NULL, &thread_add_fn, (void *) q);
    }
    pthread_create(&rm_thrd, NULL, &thread_rm_fn, (void *)q);

    usleep(wait_time * 1000000);

    for(int i = 0; i < threads; i++){
        pthread_cancel(wrk_thrds[i]);
    }
    pthread_cancel(rm_thrd);

    for(int i = 0; i < LIM; i++){
        if(check[i] < 0){
            printf("%d removed twice\n", i);
        }
    }
    
    for(int i = 0; i < threads; i++){
        pthread_join(wrk_thrds[i], NULL);
    }
    pthread_join(rm_thrd, NULL);
    printf("Done\n");
}
