                         _                         _ 
        ___ _ __ ___  __| |      _ __   ___   ___ | |
       / __| '__/ _ \/ _` |_____| '_ \ / _ \ / _ \| |
      | (__| | |  __/ (_| |_____| |_) | (_) | (_) | |
       \___|_|  \___|\__,_|     | .__/ \___/ \___/|_|
                                |_|                  

`cred-pool` is a super simple threadpool written in C.

# How to use

After allocating a pointer to `tpool_t`, there are three function calls necessary to run a pool:

1. `initialise_pool` will set up all the internal data within a `tpool_t` struct
2. `execute_job` will push a job onto the pool's internal job queue, where it will be performed by one of the waiting threads.
3. `destroy_pool` will wait until the worker threads have completed all of their work and will then free memory used by the pool.