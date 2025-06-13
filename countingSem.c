//Counting semaphores

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_RESOURCES 3
#define MAX_THREADS 5

sem_t counting_sem;

void* routine(void* arg){
    int thread_id = *(int*)arg;
    
    sem_wait(&counting_sem);
    printf("Inside the CS\n");
    sleep(2);
    printf("Exiting the CS\n");
    sem_post(&counting_sem);
    
    return NULL;
}

int main(){
    pthread_t threads[MAX_THREADS];
    int indexes[MAX_THREADS];
    int i = 0;
    
    sem_init(&counting_sem, 0, MAX_RESOURCES);
    
    for(i = 0; i < MAX_THREADS; i++){
        indexes[i] = i;
        if (pthread_create(&threads[i], NULL, routine, &indexes[i]) != 0){
            printf("Thread creation failed\n");
            return 1;
        }
    }
    
    for(i = 0; i < MAX_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&counting_sem);
    printf("Main threads at exit\n");
    return 0;
}
