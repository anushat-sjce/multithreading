// Incase you forget about function pointer

#include<stdio.h>
#include<stdlib.h>

int add (int a, int b){
    return a + b;
}

int sub(int x, int y){
    return x - y;
}

struct func{
    int data;
    int (*operations)(int, int); // function pointer declaration
};


int main(){
    int (*func_pointer)(int, int);
    func_pointer = &add;
    
    int result = func_pointer(10, 5);
    printf("%d\n", result);
    
    int (*func_sub)(int, int);
    func_sub = &sub;
    
    int sub_result = func_sub(10, 5);
    printf("Subtraction ; %d\n", sub_result);
    
    
    struct func ops;
    ops.data = 10;
    ops.operations = &add;
    
    int addition = ops.operations(10, 20);
    printf("Sum : %d\n", addition);
    
    ops.operations = &sub;
    
    int sub = ops.operations(20, 10);
    printf("Sub : %d\n", sub);
    
    return 0;
}

