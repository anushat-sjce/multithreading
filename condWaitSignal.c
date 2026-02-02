// starvation_example.c
// Compile: gcc -pthread starvation_example.c -o starvation_example
// Run as root to set real-time priorities: sudo ./starvation_example

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

int buffer = 0;// Empty = 0, Full = 1

void *producer(void *args){
    pthread_mutex_lock(&lock);
    while (buffer == 1){
        pthread_cond_wait(&cv, &lock);
    }
    buffer = 1;
    printf("Produced\n");
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&lock);
    
    return NULL;
}

void *consumer(void *args){
    pthread_mutex_lock(&lock);
    while(buffer == 0){
        pthread_cond_wait(&cv, &lock);
    }
    buffer = 0;
    printf("Consumed\n");
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&lock);
    
    return NULL;
}

int main(){
    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    
    return 0;
}
