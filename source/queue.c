#include "../include/queue.h"

struct queue_elem{
    void* data;
    struct queue_elem* next;
};

struct queue{
    struct queue_elem* first;
    struct queue_elem* last;
};


int queue_elems_free(struct queue_elem* elem);



struct queue* queue_init(void){
    struct queue* queue = (struct queue*)malloc(sizeof(struct queue));
    if(queue == NULL) return NULL;

    queue->first = NULL;
    queue->last = NULL;
    
    return queue;
}



int queue_push(struct queue* queue, void* data){
    if(queue == NULL) return -1;
    if(data == NULL) return -2;

    struct queue_elem *elem = (struct queue_elem *)malloc(sizeof(struct queue_elem));
    if(elem == NULL) return -3;
    elem->data = data;
    elem->next = NULL;

    if(queue->first == NULL && queue->last == NULL){
        queue->first = elem;
        queue->last = elem;
        return 0;
    }

    queue->last->next = elem;
    queue->last = elem;

    return 0;
}



void *queue_pop(struct queue* queue){
    if(queue == NULL) return NULL;
    if(queue->first == NULL) return NULL;

    if(queue->first == NULL) return NULL;

    void* res = queue->first->data;

    struct queue_elem* new_first = queue->first->next;
    free(queue->first);

    queue->first = new_first;

    if(queue->first == NULL) queue->last = NULL;

    return res;
}



int queue_free(struct queue* queue){
    queue_elems_free(queue->first);
    free(queue);
    return 0;
}

int queue_elems_free(struct queue_elem* elem){
    if(elem == NULL) return 0;

    queue_elems_free(elem->next);

    free(elem);
    return 0;
}