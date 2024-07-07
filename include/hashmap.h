#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"

typedef struct hashmap hashmap_t;


hashmap_t *hashmap_create(size_t size);

int hashmap_insert(hashmap_t *hashmap, char *key, void *value);

int hashmap_free(hashmap_t *hashmap);

void *hashmap_search(hashmap_t *hashmap, char *key);

#endif