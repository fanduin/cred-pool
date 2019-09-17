#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <stdlib.h>

typedef enum state { OK, SHUT_DOWN } State;

typedef void *(*function_t)(void *arg);

typedef struct job_t {
    function_t function;
    void *args;

    struct job_t *next;
} job_t;

typedef struct job_q {
    job_t *head;
    job_t *tail;

    int length;
} job_q;

void push_job(job_q *q, function_t funciton, void *args);

job_t *pop_job(job_q *q);

typedef struct {
    pthread_t thread;

    pthread_mutex_t tpool_lock;
    pthread_cond_t tpool_cond;
    pthread_cond_t tpool_done_cond;

    State state;

    job_q *q;
} tpool_t;

void initialise_pool(tpool_t *);

void destroy_pool(tpool_t *);

void execute_job(tpool_t *, function_t, void *);

#endif /* ifndef THREADPOOL_H */
