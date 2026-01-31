#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void *t1(void *args){
    pthread_mutex_lock(&m1);
    printf("Locking m1 in t1\n");
    sleep(1);
    pthread_mutex_lock(&m2);
    printf("Locking m2 in t1 \n");
    pthread_mutex_unlock(&m2);
    printf("Unlocked m2 in t1\n");
    pthread_mutex_unlock(&m1);
    printf("Unlocked m1 in t1\n");
    
    return NULL;
}

void *t2(void *args){
    pthread_mutex_lock(&m2);
    printf("Locking m2 in t2\n");
    sleep(1);
    pthread_mutex_lock(&m1);
    printf("Locking m1 in t2\n");
    pthread_mutex_unlock(&m1);
    printf("Unlocked m1 in t2\n");
    pthread_mutex_unlock(&m2);
    printf("Unlocked m2 in t2\n");
    
    return NULL;
}

int main(){
    pthread_t a, b;
    pthread_create(&a, NULL, t1, NULL);
    pthread_create(&b, NULL, t2, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    
    return 0;
}
