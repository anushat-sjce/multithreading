#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUMTHREADS 4
#define NUMTASK 10

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int a[NUMTASK]; //10 Tasks to be performed.
int in = 0; // counter for the tasks.

void* worker(void* args){
    int i = *(int *) args;
    while(1){
        pthread_mutex_lock(&lock);
        if(in >= NUMTASK){
            pthread_mutex_unlock(&lock);
            break;
        }
        int item = a[in++];
        pthread_mutex_unlock(&lock);
        
        printf("Thread %d working on task %d\n", i, item);
        sleep(1);
    }
    return 0;
}

int main(){
    pthread_t t[NUMTHREADS]; //4 Threads
    int id[NUMTHREADS]; //4 ids to be asssociated with threads
    
    for(int i = 0; i < NUMTASK; i++){
        a[i] = i+1; //Assign values to thread id's
    }
    
    for(int i = 0; i < NUMTHREADS; i++){
        id[i] = i+1;
        pthread_create(&t[i], NULL, worker, &id[i]);
    }
    
    for(int i =0; i < NUMTHREADS; i++){
        pthread_join(t[i], NULL);
    }
    
    return 0;
}
