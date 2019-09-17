#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "threadpool.h"

void push_job(job_q *q, function_t funciton, void *args) {
    job_t *job = malloc(sizeof(job_t));
    job->function = funciton;
    job->args = args;
    job->next = NULL;

    if (q->length ==  0) {
        q->head = job;
        q->tail = job;
        q->length = 1;
    } else {
        q->tail->next = job;
        q->tail = job;
        q->length++;
    }
}

job_t *pop_job(job_q *q) {
    usleep(1000);
    if (q == NULL) {
        return NULL;
    } else if (q->length == 1) {
        job_t *job = q->head;
        q->head = NULL;
        q->tail = NULL;
        q->length = 0;

        return job;
    }

    job_t *job = q->head;
    q->head = q->head->next;
    q->length--;
    return job;
}

void *handle_job(void *_pool) {
    tpool_t *pool = (tpool_t *)_pool;
    job_t *job = NULL;

    bool exit = false;

    while (true) {
        pthread_mutex_lock(&(pool->tpool_lock));

        while (pool->q->length == 0 && pool->state != SHUT_DOWN)
            pthread_cond_wait(&(pool->tpool_cond), &(pool->tpool_lock));

        switch (pool->state)
        {
        case SHUT_DOWN:
            exit = true;
            break;

        case OK:
            job = pop_job(pool->q);

            job->function(job->args);
            free(job);
            job = NULL;
            break;
        }

        pthread_cond_signal(&(pool->tpool_done_cond));
        pthread_mutex_unlock(&(pool->tpool_lock));

        if (exit == true)
            break;
    }

    return 0;
}

void initialise_pool(tpool_t *pool) {
    pthread_t *thread = &(pool->thread);

    pool->q = (job_q *) malloc(sizeof(job_q));

    pool->q->head = NULL;
    pool->q->tail = NULL;
    pool->q->length = 0;

    pool->state = OK;

    pthread_mutex_init(&(pool->tpool_lock), NULL);
    pthread_cond_init(&(pool->tpool_cond), NULL);
    pthread_cond_init(&(pool->tpool_done_cond), NULL);

    tpool_t *test_pool = (tpool_t *)malloc(sizeof(tpool_t));
    *test_pool = *pool;
    int pid = pthread_create(thread, NULL, handle_job, pool);
}

void execute_job(tpool_t *tpool, function_t function, void *args) {
    pthread_mutex_lock(&(tpool->tpool_lock));

    push_job(tpool->q, function, args);

    pthread_cond_signal(&(tpool->tpool_cond));
    pthread_mutex_unlock(&(tpool->tpool_lock));
}

void destroy_pool(tpool_t *pool) {
    pthread_mutex_lock(&(pool->tpool_lock));

    while (pool->q->length != 0)
        pthread_cond_wait(&(pool->tpool_done_cond), &(pool->tpool_lock));

    printf("destroy_pool: commencing shutdown\n");
    pool->state =  SHUT_DOWN;
    pthread_cond_broadcast(&(pool->tpool_cond));
    pthread_mutex_unlock(&(pool->tpool_lock));

    pthread_join(pool->thread, NULL);

    int pid = pthread_create(&(pool->thread), NULL, handle_job, NULL);
}
