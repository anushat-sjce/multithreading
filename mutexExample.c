//Creating mutex locks in C
#include <stdio.h>
#include <pthread.h>

//shared variable initialization
//creating a variable of type pthread_mutex
int shared_variable = 0;
pthread_mutex_t my_mutex;

void* thread_function(void* args){
    printf("In thread function\n");
    
    int i = 0;
    for(i = 0; i< 100000; i++){
        pthread_mutex_lock(&my_mutex);
        shared_variable++;
        pthread_mutex_unlock(&my_mutex);
    }
    return NULL;
}

int main(){
    pthread_t th1, th2;
    pthread_mutex_init(&my_mutex, NULL);
    int result1, result2;
    
    result1 = pthread_create(&th1, NULL, thread_function, NULL);
    if(result1 != 0){
        printf("Thread1 creation failed\n");
        return 1;
    }
    result2 = pthread_create(&th2, NULL, thread_function, NULL);
    if(result2 != 0){
        printf("Thread2 creation failed\n");
        return 1;
    }
    
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    
    printf("From the main function, Shared_Varible: %d\n", shared_variable);
    pthread_mutex_destroy(&my_mutex);
    return 0;
}
