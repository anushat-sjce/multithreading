#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;
int shareddata = 0;

void* increment(void* arg){
    for(int i = 0; i< 10; i++){
        sem_wait(&semaphore);
        printf("Inside increment\n");
        shareddata++;
        sem_post(&semaphore);
    }
    return NULL;
}

void* decrement(void* arg){
    for(int i = 0; i < 10; i++){
        sem_wait(&semaphore);
        printf("Inside decrement\n");
        shareddata--;
        sem_post(&semaphore);
    }
    return NULL;
}

int main(){
    pthread_t p1, p2;
    
    sem_init(&semaphore, 0, 1);
    
    int result1 = pthread_create(&p1, NULL, increment, NULL);
    if(result1 != 0){
        printf("p1 thread creation failed\n");
        return 1;
    }
    
    int result2 = pthread_create(&p2, NULL, decrement, NULL);
    if (result2 != 0){
        printf("p2 thread creation failed\n");
        return 1;
    }
    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    
    printf("Shared value : %d\n", shareddata);
    
    sem_destroy(&semaphore);
    
    return 0;
}
