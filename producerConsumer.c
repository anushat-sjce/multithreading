#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define ITEM 10

int count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t cons = PTHREAD_COND_INITIALIZER;

void* producer(void* args){
    for(int i = 0; i < ITEM; i++){
        pthread_mutex_lock(&lock);
        while(count == ITEM){
            pthread_cond_wait(&prod, &lock);
        }
        count++;
        printf("Counter %d produced item %d\n", i, count);
        pthread_cond_signal(&cons);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return 0;
}

void* consumer(void* args){
    for(int i = 0; i < ITEM; i++){
        pthread_mutex_lock(&lock);
        while(count == 0){
            pthread_cond_wait(&cons, &lock);
        }
        count--;
        printf("Counter %d consumed item %d\n", i, count);
        pthread_cond_signal(&prod);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return 0;
}

int main(){
    pthread_t t1, t2;
    
    int r1 = pthread_create(&t1, NULL, &producer, NULL);
    int r2 = pthread_create(&t2, NULL, &consumer, NULL);
    
    if((r1 || r2) != 0){
        printf("Error during thread creation\n");
        exit(0);
    }
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    return 0;
}
