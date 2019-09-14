#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

#include "threadpool.h"

bool job_ready;

pthread_mutex_t job_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_ready_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t job_done_c = PTHREAD_COND_INITIALIZER;

job_t job;

void *handle_job() {
    pthread_mutex_lock(&job_lock);

    while (job_ready == false)
        pthread_cond_wait(&job_ready_c, &job_lock);

    job(NULL);

    job_ready = false;
    pthread_cond_signal(&job_done_c);
    pthread_mutex_unlock(&job_lock);

    return 0;
}

void initialise_pool(tpool_t *pool) {
    pthread_t *thread = &(pool->thread);

    int pid = pthread_create(thread, NULL, handle_job, NULL);
}

void execute_job(tpool_t *tpool, job_t new_job) {
    pthread_mutex_lock(&job_lock);

    job = new_job;
    job_ready = true;

    pthread_cond_signal(&job_ready_c);
    pthread_mutex_unlock(&job_lock);
}

void destroy_pool(tpool_t *pool) {
    pthread_cond_wait(&job_done_c, &job_lock);

    pthread_join(pool->thread, NULL);
}
