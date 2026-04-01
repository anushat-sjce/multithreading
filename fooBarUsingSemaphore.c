#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

int n = 10;
sem_t foo, bar;

void* foofunc(void* args){
    for(int i = 0; i< n ; i++){
        sem_wait(&foo);
        printf("foo\n");
        sem_post(&bar);
    }   
    return 0;
}

void* barfunc(void* args){
    for(int i = 0; i < n;i++){
        sem_wait(&bar);
        printf("bar\n");
        sem_post(&foo);
    }
    return 0;
}

int main(){
    pthread_t t1, t2;
    sem_init(&foo, 0, 1);
    sem_init(&bar, 0, 0);
    
    int r1 = pthread_create(&t1, NULL, &foofunc, NULL);
    int r2 = pthread_create(&t2, NULL, &barfunc, NULL);
    
    if((r1 || r2) != 0){
        printf("Error during thread creation\n");
        return 0;
    }
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    sem_destroy(&foo);
    sem_destroy(&bar);
    
    return 0;
}
