#ifndef THREADPOOL_H
#define THREADPOOL_H

typedef void *(*job_t)(void *);

void execute_job(job_t);

#endif /* ifndef THREADPOOL_H */
