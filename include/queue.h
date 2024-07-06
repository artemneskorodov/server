#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

struct queue;

struct queue* queue_init(void);

int queue_push(struct queue* queue, void* data);

void *queue_pop(struct queue* queue);

int queue_free(struct queue* queue);

#endif