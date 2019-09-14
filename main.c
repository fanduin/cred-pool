#include <stdio.h>
#include <time.h>

#include "threadpool.h"

void *hello()
{
    printf("\thello from another thread\n");

    return 0;
}

int main()
{
    job_t h = hello;

    tpool_t pool = {};
    initialise_pool(&pool);

    for (int i = 0; i < 5; i++)
    {
        execute_job(&pool, h);
    }
    
    destroy_pool(&pool);

    return 0;
}