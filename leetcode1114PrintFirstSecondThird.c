#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock;
pthread_cond_t cond;

int step = 1;

void* first(void* args){
    pthread_mutex_lock(&lock);
    printf("First\n");
    step = 2;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
    
    return 0;
}

void* second(void* args){
    pthread_mutex_lock(&lock);
    while(step != 2) pthread_cond_wait(&cond, &lock);
    printf("second\n");
    step = 3;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
    
    return 0;
}

void* third(void* args){
    pthread_mutex_lock(&lock);
    while(step != 3) pthread_cond_wait(&cond, &lock);
    printf("third\n");
    pthread_mutex_unlock(&lock);
    
    return 0;
}

int main(){
    pthread_t t1, t2, t3;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    
    int r1 = pthread_create(&t3, NULL, third, NULL);
    int r2 = pthread_create(&t1, NULL, first, NULL);
    int r3 = pthread_create(&t2, NULL, second, NULL);
    
    if((r1 || r2 || r3) != 0){
        printf("Error during pthread creation\n");
        exit(0);
    }
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    
    return 0;
}
