#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

typedef enum message { EMPTY, JOB_READY, SHUT_DOWN } Message;

typedef void *(*job_t)(void *arg);

typedef struct tpool_t {
    pthread_t thread;
} tpool_t;

void initialise_pool(tpool_t *);

void destroy_pool(tpool_t *);

void execute_job(tpool_t *, job_t);

#endif /* ifndef THREADPOOL_H */
