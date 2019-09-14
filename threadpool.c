#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

#include "threadpool.h"

bool job_ready;

pthread_mutex_t job_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_ready_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t job_done_c = PTHREAD_COND_INITIALIZER;

job_t job;

void *handle_job(void *_job) {
    job_t work = *(job_t *)_job;

    work(NULL);

    return 0;
}

void initialise_pool(tpool_t *pool) {
}

void execute_job(tpool_t *tpool, job_t new_job) {
    pthread_t *thread = &(tpool->thread);

    int pid = pthread_create(thread, NULL, handle_job, &new_job);

    pthread_join(*thread, NULL);
}

void destroy_pool(tpool_t *pool) {
}
