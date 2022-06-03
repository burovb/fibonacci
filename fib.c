#include "fib.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



int fib(int num) {
    if (num < 0)
        return 0;
    if (num == 0 || num == 1)
        return num;

    return fib(num - 1) + fib(num - 2);
}

void *func(void *input) {
    *(((args_t *)input)->res) += fib(((args_t *)input)->num);

    return NULL;
}

args_t *init(int input, int *shm) {
    args_t *arr = malloc(sizeof *arr * 2);
    for (size_t i = 0; i < 2; i++) {
        (arr + i)->res = shm;
        (arr + i)->num = input - (i + 1);
    }
    
    return arr;
}