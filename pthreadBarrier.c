#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

pthread_barrier_t barrier;

void* worker(void* arg) {
    int id = (int)(long)arg;
    printf("Thread %d: stage 1\n", id);
    // do stage 1 work...
    pthread_barrier_wait(&barrier); // wait for all threads
    printf("Thread %d: stage 2\n", id);
    // do stage 2 work...
    return NULL;
}

int main(void) {
    pthread_t t[NTHREADS];
    pthread_barrier_init(&barrier, NULL, NTHREADS);
    for (long i = 0; i < NTHREADS; ++i) pthread_create(&t[i], NULL, worker, (void*)i);
    for (int i = 0; i < NTHREADS; ++i) pthread_join(t[i], NULL);
    pthread_barrier_destroy(&barrier);
    return 0;
}
