#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <stdlib.h>

// Possible states for a pool to be in
typedef enum state { OK, SHUT_DOWN } State;

// Wrapper to basic function pointer
typedef void *(*function_t)(void *args);

// Struct defining a single job that will be part of a queue.
// Jobs have a function, a generic argument and a pointer to
// the next job in the queue.
typedef struct job_t {
    function_t function;
    void *args;

    struct job_t *next;
} job_t;

// Job Queue struct.
// It holds the beginning and end of the queue,
// as well as the current length of the queue.
typedef struct job_q {
    job_t *head;
    job_t *tail;

    int length;
} job_q;

// Add a function and its argument(s) to the job queue
void push_job(job_q *q, function_t function, void *args);

// Remove the next job from the queue, returing a pointer to
// the removed job.
job_t *pop_job(job_q *q);

// Main threadpool struct.
// Contains a list of threads and thread count
// as well as the mutex and condition variables to control
// pool data (mostly queue) access, the current pool state
// and the job queue.
typedef struct {
    pthread_t *thread;
    size_t nthreads;

    pthread_mutex_t tpool_lock;
    pthread_cond_t tpool_cond;
    pthread_cond_t tpool_done_cond;

    State state;

    job_q *q;
} tpool_t;

// Set up a pool structure and create the necessary threads.
void initialise_pool(tpool_t *pool, size_t n);

// Send shut down message to threads, wait for them to finish
// and then free memory.
void destroy_pool(tpool_t *pool);

// Takes care of the business logic with passing work to the pool.
void execute_job(tpool_t *pool, function_t function, void *args);

#endif /* ifndef THREADPOOL_H */
