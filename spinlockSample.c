/* Four Pthreads incrementing the shared variable from 0 to 16
   sequentially. Making use of spinlock.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define ITERATIONS 4

pthread_spinlock_t lock;
int shared_var = 0;

void* worker(void* x){
    long id = (long)(x);
    for(int i = 0; i < ITERATIONS; i++){
        pthread_spin_lock(&lock);
        shared_var++;
        printf("Thread %ld incremented shared var to %d\n", x, shared_var);
        pthread_spin_unlock(&lock);
    }
    
    return 0;
}

int main(){
    pthread_t t[4];
    pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);
    
    for(long i = 1; i <=4 ; i++){
        int r1 = pthread_create(&t[i], NULL, worker, (void*)i);
    
        if(r1 != 0) {
            printf("Error during thread creation\n");
            exit(0);
        }
    }
    for(int j = 0 ; j< 4; j++){
        pthread_join(t[j], NULL);
    }
    
    pthread_spin_destroy(&lock);
    
    return 0;
}
