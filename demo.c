#include <stdio.h>
#include <time.h>

#include "threadpool.h"

void *print_sum(void *_n) {
    int n = *(int *)_n;

    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
    }

    printf("The sum from 1 to %d is %d\n", n, sum);

    return 0;
}

int main()
{
    int nthreads = 5;
    int njobs = 50;

    tpool_t *pool = (tpool_t *)malloc(sizeof(tpool_t));
    initialise_pool(pool, nthreads);

    for (int i = 0; i < 2; i++) {
        printf("Thread %d: %lu\n", i, pool->thread[i]);
    }

    int *numbers = (int *)malloc(50 * sizeof(int));
    for (int i = 0; i < 50; i++) {
        numbers[i] = i;
        execute_job(pool, print_sum, &numbers[i]);
    }

    destroy_pool(pool);

    free(numbers);

    return 0;
}