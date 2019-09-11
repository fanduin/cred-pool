#include <stdio.h>

#include "threadpool.h"

void *hello()
{
    printf("hello from another thread\n");
}

int main()
{
    job_t h = hello;

    printf("hello from before execution\n");
    execute_job(h);
    printf("hello from after execution\n");

    return 0;
}