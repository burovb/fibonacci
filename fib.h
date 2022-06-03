typedef struct args {
	int num;
	int *res;
} args_t;



int fib(int num);

void *func(void *input);

args_t *init(int input, int *shm);