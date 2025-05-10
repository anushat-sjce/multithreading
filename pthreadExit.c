// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

static void* thread_fn_callback(void* arg){
    char* input = (char*) arg;
    int a = 0;
    
    while(a < 10 ){
        printf("%s\n", input);
        sleep(1);
        if (a == 5){
            pthread_exit(0);
        }
        a++;
    }
}


void thread1_create(){
    
    pthread_t thread1;

    static char* pt1 = "Am thread one\n";
    
    int rc = pthread_create(&thread1, NULL, thread_fn_callback, (void *) pt1);
    
    if (rc != 0) {
        printf("Error occured, thread couldnt be created, errNo: %d\n", rc);
        exit(0);
    }
}

int main(int argc, char **argv) {
    
    thread1_create();
    printf("Pausing the main thread\n");
    pause();
    
    return 0;
}
