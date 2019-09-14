#include <stdio.h>
#include <time.h>

#include "threadpool.h"

void *hello()
{
    printf("hello from another thread\n");

    return 0;
}

void delay(int seconds) {
    clock_t start_time = clock();

    while (clock() < start_time + seconds * CLOCKS_PER_SEC)
        ;
}

int main()
{
    job_t h = hello;

    tpool_t pool = {};
    initialise_pool(&pool);

    delay(5);
    execute_job(&pool, h);

    destroy_pool(&pool);

    return 0;
}