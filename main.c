#include <stdio.h>
#include <time.h>

#include "threadpool.h"

void *hello(void *_name)
{
    char *name = (char *)_name;
    printf("\thello, %s, from another thread\n", name);

    return 0;
}

void *print_sum(void *_n) {
    int n = *(int *)_n;

    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
    }

    printf("The sum from 1 to %d is %d\n", n, sum);

    return 0;
}

char *a = "A";
char *b = "B";
char *c = "C";
char *d = "D";

int main()
{
    tpool_t *pool = (tpool_t *)malloc(sizeof(tpool_t));
    initialise_pool(pool);

    execute_job(pool, hello, a);
    execute_job(pool, hello, b);
    execute_job(pool, hello, c);
    execute_job(pool, hello, d);

    destroy_pool(pool);

    return 0;
}