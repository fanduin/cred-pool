#include <pthread.h>
#include <stdio.h>

#include "threadpool.h"

void execute_job(job_t job) {
    pthread_t thread;

    int pid = pthread_create(&thread, NULL, job, NULL);

    pthread_join(thread, NULL);
}
