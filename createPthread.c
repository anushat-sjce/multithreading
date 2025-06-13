#include <stdio.h>
#include <pthread.h>

void* thread_function(void* arg) {
    printf("Hello from the thread!\n");
    return NULL;
}

int main() {
    pthread_t thread_id;
    int result;

    result = pthread_create(&thread_id, NULL, thread_function, NULL);
    if (result != 0) {
        perror("Thread creation failed");
        return 1;
    }

    printf("Hello from the main thread!\n");

    //pthread_join(thread_id, NULL); // Wait for the thread to finish
    return 0;
}
