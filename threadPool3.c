#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3
#define NUM_TASKS   6

// Simplified Task: just an integer payload
typedef struct {
    int payload;
} Task;

Task task_queue[NUM_TASKS];
int task_count = 0;

pthread_mutex_t lock;
pthread_cond_t cond;

void *worker(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (task_count == 0) {
            pthread_cond_wait(&cond, &lock);
        }
        // Fetch task
        Task task = task_queue[--task_count];
        pthread_mutex_unlock(&lock);

        // Process task (here just print payload)
        printf("Thread %lu processing task with payload %d\n",
               pthread_self(), task.payload);
        sleep(1); // simulate work
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    // Create worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    // Submit tasks
    for (int i = 0; i < NUM_TASKS; i++) {
        pthread_mutex_lock(&lock);
        task_queue[task_count++] = (Task){i};  // payload = i
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }

    // Allow time for tasks to finish
    sleep(5);

    return 0;
}
