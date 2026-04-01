#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

int n = 5;
sem_t zero, even, odd;

void* printZero(void* args){
    for(int i = 1; i<= n ; i++){
        sem_wait(&zero);
        printf("0\n");
        fflush(stdout);
        if(i % 2 == 0){
            sem_post(&even);
        } else {
            sem_post(&odd);
        }
    }   
    return 0;
}

void* printEven(void* args){
    for(int i = 2; i <= n;i=i+2){
        sem_wait(&even);
        printf("%d\n", i);
        fflush(stdout);
        sem_post(&zero);
    }
    return 0;
}

void* printOdd(void* args){
    for(int i = 1; i <= n; i=i+2){
        sem_wait(&odd);
        printf("%d\n", i);
        fflush(stdout);
        sem_post(&zero);
    }
}

int main(){
    pthread_t t1, t2, t3;
    sem_init(&zero, 0, 1);
    sem_init(&even, 0, 0);
    sem_init(&odd, 0, 0);
    
    int r1 = pthread_create(&t1, NULL, &printZero, NULL);
    int r2 = pthread_create(&t2, NULL, &printEven, NULL);
    int r3 = pthread_create(&t3, NULL, &printOdd, NULL);
    
    if((r1 || r2 || r3) != 0){
        printf("Error during thread creation\n");
        return 0;
    }
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    
    sem_destroy(&zero);
    sem_destroy(&even);
    sem_destroy(&odd);
    
    return 0;
}
