#include <stdio.h>
#include <pthread.h>

void* routine(void* args){
    int thread_id = *(int*)args;
    printf("Hello from child thread %d\n", thread_id);
    return NULL;
}

int main(){
    pthread_t threads[10];
    int threads_id[10];
    int result;
    int i = 0;
    
    for(i = 0; i < 10 ; i++){
        threads_id[i] = i;
        result = pthread_create(&threads[i], NULL, routine, &threads_id[i]);
        if (result != 0){
            printf("Pthread creation failed\n");
            return 1;
        }
    }
    
    printf("Hello from main thread\n");
    
    for(i = 0; i < 10 ; i++){
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}
