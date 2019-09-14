#include <stdio.h>

#include "threadpool.h"

void *hello()
{
    printf("hello from another thread\n");

    return 0;
}

int main()
{
    job_t h = hello;

    tpool_t pool = {};
    initialise_pool(&pool);

    execute_job(&pool, h);

    destroy_pool(&pool);

    return 0;
}