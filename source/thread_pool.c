#include "../include/thread_pool.h"


void* worker(void* args);


struct thread_pool{
    int threads_number;
    pthread_t* threads;
    pthread_mutex_t mutex;
    struct queue* queue;
    void (*thread_function)(void *args);
    int active_flag;
};

struct thread_pool* thread_pool_init(int threads_number, void (*thread_function)(void *args)){

    if(thread_function == NULL) return NULL;

    if(threads_number < 1) return NULL;


    struct thread_pool* thread_pool = (struct thread_pool *)malloc(sizeof(struct thread_pool));

    if(thread_pool == NULL) return NULL;


    thread_pool->threads_number = threads_number;
    thread_pool->thread_function = thread_function;


    thread_pool->queue = queue_init();
    if(thread_pool->queue == NULL){
        free(thread_pool);
        return NULL;
    }


    pthread_mutex_init(&thread_pool->mutex, NULL);


    thread_pool->active_flag = 1;


    thread_pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * threads_number);
    if(thread_pool->threads == NULL){
        queue_free(thread_pool->queue);
        free(thread_pool);
        return NULL;
    }


    for(int i = 0; i < threads_number; i++){
        if(pthread_create(thread_pool->threads + i, NULL, &worker, (void *)thread_pool) != 0){

            queue_free(thread_pool->queue);
            thread_pool->active_flag = 0;
            for(int i = 0; i < threads_number; i++) pthread_join(thread_pool->threads[i], NULL);
            free(thread_pool->threads);
            free(thread_pool);
            return NULL;

        }
    }


    return thread_pool;

}



int thread_pool_add_job(struct thread_pool *thread_pool, void *job){
    pthread_mutex_lock(&thread_pool->mutex);
    if(queue_push(thread_pool->queue, job) != 0) return -1;
    pthread_mutex_unlock(&thread_pool->mutex);
    return 0;
}



int thread_pool_free(struct thread_pool* thread_pool){
    thread_pool->active_flag = 0;

    for(int i = 0; i < thread_pool->threads_number; i++){
        pthread_join(thread_pool->threads[i], NULL);
    }


    queue_free(thread_pool->queue);
    free(thread_pool->threads);
    free(thread_pool);

    return 0;
}



void* worker(void* args){

    struct thread_pool* thread_pool = (struct thread_pool *)args;

    if(thread_pool == NULL) pthread_exit(NULL);

    while(!0){
        
        if(thread_pool->active_flag != 1){
            pthread_exit(NULL);
        }

        pthread_mutex_lock(&thread_pool->mutex);
        
        void* job = queue_pop(thread_pool->queue);
        pthread_mutex_unlock(&thread_pool->mutex);

        if(job == NULL) continue;

        thread_pool->thread_function(job);

    }

    return NULL;
}