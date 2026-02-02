// starvation_example.c
// Compile: gcc -pthread starvation_example.c -o starvation_example
// Run as root to set real-time priorities: sudo ./starvation_example

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *high_priority_worker(void *arg) {
    while(1){
        pthread_mutex_lock(&lock);
        usleep(100);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *low_priority_task(void *arg) {
    while(1){
        pthread_mutex_lock(&lock);
        sleep(5);
        pthread_mutex_unlock(&lock);
    }
}

int set_realtime_priority(pthread_t t, int prio) {
   struct sched_param sp;
   sp.sched_priority = prio;
   return pthread_setschedparam(t, SCHED_FIFO, &sp);
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, high_priority_worker, NULL);
    pthread_create(&t2, NULL, low_priority_task, NULL);
    
    set_realtime_priority(t1, 80);
    set_realtime_priority(t2, 20);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    return 0;
}
