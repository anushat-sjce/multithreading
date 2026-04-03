#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3
#define QUEUE_SIZE  5
#define NUM_EVENTS  12

// Shared queue
int queue[QUEUE_SIZE];
int front = 0, rear = 0, count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full  = PTHREAD_COND_INITIALIZER;

// Producer: generates log events
void* producer(void* arg) {
    for (int i = 1; i <= NUM_EVENTS; i++) {
        pthread_mutex_lock(&lock);

        while (count == QUEUE_SIZE) {
            // Queue full → wait
            pthread_cond_wait(&not_full, &lock);
        }

        queue[rear] = i;
        rear = (rear + 1) % QUEUE_SIZE;
        count++;

        printf("[Producer] Generated log event %d\n", i);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);

        usleep(200000); // simulate fast event generation
    }
    return NULL;
}

// Consumer: thread pool workers writing logs
void* consumer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&lock);

        while (count == 0) {
            // Queue empty → wait
            pthread_cond_wait(&not_empty, &lock);
        }

        int event = queue[front];
        front = (front + 1) % QUEUE_SIZE;
        count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);

        // Process log event (simulate disk write)
        printf("Thread %d writing log event %d\n", id, event);
        sleep(1); // simulate slower disk I/O
    }
    return NULL;
}

int main() {
    pthread_t prod, threads[NUM_THREADS];
    int ids[NUM_THREADS];

    // Start producer
    pthread_create(&prod, NULL, producer, NULL);

    // Start consumer threads (thread pool)
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, consumer, &ids[i]);
    }

    pthread_join(prod, NULL);
    // In real systems, you’d signal consumers to exit gracefully
    sleep(10); // let consumers finish
    return 0;
}
