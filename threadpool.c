#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

#include "threadpool.h"

bool msg_ready;
bool reading_msg;
pthread_mutex_t msg_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t msg_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t msg_read_cond = PTHREAD_COND_INITIALIZER;

job_t job;
Message message;

void *handle_job() {
    bool exit = false;

    while (true) {
        printf("handler: locking msg mutex\n");
        pthread_mutex_lock(&msg_lock);

        printf("handler: waiting for message\n");
        while (msg_ready == false)
            pthread_cond_wait(&msg_cond, &msg_lock);

        reading_msg = true;
        
        printf("handler: processing message\n");
        switch (message)
        {
        case JOB_READY:
            printf("handler: job ready message\n");
            job(NULL);
            break;
        
        case SHUT_DOWN:
            printf("handler: shut down message\n");
            exit = true;
            break;

        case EMPTY:
            printf("handler: empty message\n");
            break;
        }

        msg_ready = false;
        reading_msg = false;
        pthread_cond_signal(&msg_read_cond);
        pthread_mutex_unlock(&msg_lock);

        if (exit == true)
            break;
    }

    return 0;
}

void initialise_pool(tpool_t *pool) {
    pthread_t *thread = &(pool->thread);

    int pid = pthread_create(thread, NULL, handle_job, NULL);

    message = EMPTY;
    reading_msg = false;
}

void execute_job(tpool_t *tpool, job_t new_job) {
    printf("execute_job: locking msg mutex\n");
    pthread_mutex_lock(&msg_lock);

    while (msg_ready == true) {
        printf("execute_job: waiting on condition\n");
        pthread_cond_wait(&msg_read_cond, &msg_lock);
    }
    
    reading_msg = false;
    job = new_job;
    message = JOB_READY;
    msg_ready = true;

    printf("execute_job: signalling handler\n");
    pthread_cond_signal(&msg_cond);
    printf("execute_job: unlocking msg mutex\n");
    pthread_mutex_unlock(&msg_lock);
}

void destroy_pool(tpool_t *pool) {
    printf("destroy_pool: locking msg mutex\n");
    pthread_mutex_lock(&msg_lock);

    message =  SHUT_DOWN;
    printf("destroy_pool: broadcasting to handler\n");
    pthread_cond_broadcast(&msg_cond);
    pthread_mutex_unlock(&msg_lock);

    printf("destroy_pool: joining handler\n");
    pthread_join(pool->thread, NULL);
}
