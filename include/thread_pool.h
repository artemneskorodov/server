#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct thread_pool;

struct thread_pool* thread_pool_init(int threads_number, void (*thread_function)(void *args));

int thread_pool_add_job(struct thread_pool *thread_pool, void *job);

int thread_pool_free(struct thread_pool* thread_pool);

#endif