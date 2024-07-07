#include "../include/hashmap.h"

struct hashmap{
    size_t size;
    struct binary_tree **trees;
};



size_t hash_func(char *key, size_t map_size){

    size_t hash = 0;
    size_t key_length = strlen(key);

    for(size_t index = 0; index < key_length; index++){
        hash = key[index] + 31 * hash;
        hash %= map_size;
    }

    return hash;

}



hashmap_t *hashmap_create(size_t size){
    
    hashmap_t *hashmap = (hashmap_t *)malloc(sizeof(hashmap_t));

    if(hashmap == NULL) return NULL;


    hashmap->size = size;

    hashmap->trees = (struct binary_tree **)malloc(sizeof(struct binary_tree *) * size);

    if(hashmap->trees == NULL){
        free(hashmap);
        return NULL;
    }

    for(size_t index = 0; index < size; index++){
        hashmap->trees[index] = binary_tree_init(&compare_strings);
        if(hashmap->trees[index] == NULL){
            for(size_t i = 0; i < index; i++){
                free_binary_tree(hashmap->trees[index]);
                free(hashmap->trees);
                free(hashmap);
                return NULL;
            }
        }
    }

    return hashmap;

}


int hashmap_insert(hashmap_t *hashmap, char *key, void *value){

    size_t hash = hash_func(key, hashmap->size);

    printf("%lld\n", hash);

    return insert_binary_tree(hashmap->trees[hash], key, value);

}



int hashmap_free(hashmap_t *hashmap){
    for(size_t index = 0; index < hashmap->size; index++){
        free_binary_tree(hashmap->trees[index]);
    }

    free(hashmap->trees);
    free(hashmap);
    return 0;
}



void *hashmap_search(hashmap_t *hashmap, char *key){

    size_t hash = hash_func(key, hashmap->size);

    return search_binary_tree(hashmap->trees[hash], key);

}