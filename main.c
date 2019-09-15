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

char *r = "Raphael";
char *m = "Michaelangelo";
char *l = "Leonardo";
char *d = "Donnatello";

int main()
{
    job_t j = {
        hello,
    };

    tpool_t pool = {};
    initialise_pool(&pool);

    j.args = r;
    execute_job(&pool, j);

    j.args = m;
    execute_job(&pool, j);

    j.args = l;
    execute_job(&pool, j);

    j.function = print_sum;
    int test_n = 100;
    j.args = &test_n;
    execute_job(&pool, j);

    j.function = hello;
    j.args = d;
    execute_job(&pool, j);

    destroy_pool(&pool);

    return 0;
}