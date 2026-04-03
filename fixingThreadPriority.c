#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* low_priority(void* arg) {
    struct sched_param param;

    pthread_mutex_lock(&lock);
    printf("[Low] acquired lock, doing long work...\n");

    // Simulate priority inheritance: boost priority while holding lock
    param.sched_priority = 60; // temporarily raise to high priority
    pthread_setschedparam(pthread_self(), SCHED_RR, &param);

    sleep(5); // simulate long work

    // Restore original priority
    param.sched_priority = 20;
    pthread_setschedparam(pthread_self(), SCHED_RR, &param);

    pthread_mutex_unlock(&lock);
    printf("[Low] released lock.\n");
    return NULL;
}

void* high_priority(void* arg) {
    sleep(1); // let low acquire lock first
    printf("[High] trying to acquire lock...\n");
    pthread_mutex_lock(&lock);
    printf("[High] acquired lock!\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* medium_priority(void* arg) {
    sleep(2); // runs while high is waiting
    printf("[Medium] running independent work...\n");
    sleep(3);
    printf("[Medium] finished work.\n");
    return NULL;
}

int main() {
    pthread_t low, high, medium;
    pthread_attr_t attr_low, attr_high, attr_medium;
    struct sched_param param_low, param_high, param_medium;

    pthread_attr_init(&attr_low);
    pthread_attr_init(&attr_high);
    pthread_attr_init(&attr_medium);

    pthread_attr_setschedpolicy(&attr_low, SCHED_RR);
    pthread_attr_setschedpolicy(&attr_high, SCHED_RR);
    pthread_attr_setschedpolicy(&attr_medium, SCHED_RR);

    param_low.sched_priority = 20;
    param_medium.sched_priority = 40;
    param_high.sched_priority = 60;

    pthread_attr_setschedparam(&attr_low, &param_low);
    pthread_attr_setschedparam(&attr_medium, &param_medium);
    pthread_attr_setschedparam(&attr_high, &param_high);

    pthread_attr_setinheritsched(&attr_low, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setinheritsched(&attr_medium, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setinheritsched(&attr_high, PTHREAD_EXPLICIT_SCHED);

    pthread_create(&low, &attr_low, low_priority, NULL);
    pthread_create(&high, &attr_high, high_priority, NULL);
    pthread_create(&medium, &attr_medium, medium_priority, NULL);

    pthread_join(low, NULL);
    pthread_join(high, NULL);
    pthread_join(medium, NULL);

    return 0;
}
