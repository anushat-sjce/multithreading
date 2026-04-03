#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 10

int buffer[BUFFER_SIZE];
int count = 0; // number of items in buffer
int done = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            // Buffer full → wait until consumer signals
            pthread_cond_wait(&cond_producer, &mutex);
        }

        // Produce item
        buffer[count++] = i;
        printf("Producer produced item %d (count=%d)\n", i, count);

        // Signal consumer that item is available
        pthread_cond_signal(&cond_consumer);

        pthread_mutex_unlock(&mutex);
        sleep(1); // simulate production delay
    }
    done = 1;
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        pthread_mutex_lock(&mutex);

        while ((count == 0) && (done != 1)) {
            // Buffer empty → wait until producer signals
            pthread_cond_wait(&cond_consumer, &mutex);
        }
       
       if ((count == 0) && (done == 1)){
           printf("No more items to be consumed \n");
           exit(0);
        }
        // Consume item
        int item = buffer[--count];
        printf("Consumer consumed item %d (count=%d)\n", item, count);

        // Signal producer that space is available
        pthread_cond_signal(&cond_producer);

        pthread_mutex_unlock(&mutex);
        sleep(2); // simulate consumption delay
    }
    return NULL;
}

int main() {
    pthread_t prod, cons, t1;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    pthread_create(&t1, NULL, consumer, NULL);
    
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    pthread_join(t1, NULL);
    
    printf("All items produced and consumed successfully.\n");
    return 0;
}
