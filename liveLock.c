// livelock_demo.c
// Compile: gcc -pthread livelock_demo.c -o livelock_demo
// Run: ./livelock_demo

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t resA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resB = PTHREAD_MUTEX_INITIALIZER;

void *worker1(void *arg) {
    while (1) {
        // try to take A then B; on conflict release and retry immediately
        if (pthread_mutex_trylock(&resA) == 0) {
            printf("W1: locked A\n");
            usleep(100000); // simulate work
            if (pthread_mutex_trylock(&resB) == 0) {
                printf("W1: locked B -> doing work\n");
                pthread_mutex_unlock(&resB);
                pthread_mutex_unlock(&resA);
                break;
            } else {
                // conflict: release A and retry immediately -> can cause livelock
                printf("W1: failed to lock B, releasing A and retrying\n");
                pthread_mutex_unlock(&resA);
            }
        }
        // yield to other thread
        sched_yield();
    }
    return NULL;
}

void *worker2(void *arg) {
    while (1) {
        // symmetric behavior: try B then A
        if (pthread_mutex_trylock(&resB) == 0) {
            printf("W2: locked B\n");
            usleep(100000);
            if (pthread_mutex_trylock(&resA) == 0) {
                printf("W2: locked A -> doing work\n");
                pthread_mutex_unlock(&resA);
                pthread_mutex_unlock(&resB);
                break;
            } else {
                printf("W2: failed to lock A, releasing B and retrying\n");
                pthread_mutex_unlock(&resB);
            }
        }
        sched_yield();
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker1, NULL);
    pthread_create(&t2, NULL, worker2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Both workers finished\n");
    return 0;
}
