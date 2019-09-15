#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

typedef enum message { EMPTY, JOB_READY, SHUT_DOWN } Message;

typedef void *(*function_t)(void *arg);

typedef struct {
    function_t function;
    void *args;
} job_t;

typedef struct {
    pthread_t thread;
} tpool_t;

void initialise_pool(tpool_t *);

void destroy_pool(tpool_t *);

void execute_job(tpool_t *, job_t);

#endif /* ifndef THREADPOOL_H */
