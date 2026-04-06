#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3
#define QUEUE_SIZE  8

typedef struct {
    int payload;
} Task;

typedef struct {
    Task tasks[QUEUE_SIZE];
    int front;
    int rear;
    int count;
} TaskQueue;

TaskQueue queue;
pthread_mutex_t lock;
pthread_cond_t cond;

void queue_init(TaskQueue *q) {
    q->front = q->rear = q->count = 0;
}

int queue_is_empty(TaskQueue *q) {
    return q->count == 0;
}

int queue_is_full(TaskQueue *q) {
    return q->count == QUEUE_SIZE;
}

void queue_push(TaskQueue *q, Task t) {
    if (queue_is_full(q)) return;
    q->tasks[q->rear] = t;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->count++;
}

Task queue_pop(TaskQueue *q) {
    Task t = q->tasks[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    return t;
}

void *worker(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (queue_is_empty(&queue)) {
            pthread_cond_wait(&cond, &lock);
        }
        Task task = queue_pop(&queue);
        pthread_mutex_unlock(&lock);

        printf("Thread %lu processing task %d\n", pthread_self(), task.payload);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    queue_init(&queue);

    // Create worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    // Submit tasks
    for (int i = 0; i < 12; i++) {  // more than QUEUE_SIZE to test circular behavior
        pthread_mutex_lock(&lock);
        if (!queue_is_full(&queue)) {
            queue_push(&queue, (Task){i});
            pthread_cond_signal(&cond);
        } else {
            printf("Queue full, skipping task %d\n", i);
        }
        pthread_mutex_unlock(&lock);
        sleep(0.2);
    }

    sleep(5);
    return 0;
}
