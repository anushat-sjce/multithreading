#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void *t1(void *args){
    int locked_m1 = 0, locked_m2 = 0; 
    if (pthread_mutex_trylock(&m1) == 0) {
        printf("locked m1 in t1\n");
        locked_m1 = 1;
        sleep(2);
        if (pthread_mutex_trylock(&m2) == 0) {
            printf("Locked m2 in t1 \n");
            locked_m2 = 1;
        } else {
            printf("Failed to lock m2\n");
        }
    } else {
        printf("Failed to lock m1\n");
    }
    
    if(locked_m2 == 1){
        pthread_mutex_unlock(&m2);
        printf("Unlocked m2 in t1\n");
    }
    
    if(locked_m1 == 1){
        pthread_mutex_unlock(&m1);
        printf("Unlocked m1 in t1\n");
    }
    return NULL;
}

void *t2(void *args){
    int locked_m1 = 0, locked_m2 = 0;
    if (pthread_mutex_trylock(&m2) == 0){
        printf("locked m2 in t2\n");
        locked_m2 =1;
        sleep(2);
        if(pthread_mutex_trylock(&m1) == 0){
            printf("Locking m1 in t2\n");
            locked_m1 = 1;
        }
    } else {
        printf("Locking m2 failed\n");
    }
    
    if(locked_m1 == 1){
        printf("Unlocked m1 in t2\n");
    }
    
    if(locked_m2 == 1){
        printf("Unlocked m2 in t2\n");
    }
    
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
