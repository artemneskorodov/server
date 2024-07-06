#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct binary_tree;
struct node_binary_tree;

struct binary_tree* binary_tree_init(int (*compare_function)(void* first_key, void* second_key));

int insert_binary_tree(struct binary_tree* tree, void* key, void* data);

void* search_binary_tree(struct binary_tree* tree, void* key);

int free_binary_tree(struct binary_tree* tree);



int compare_strings(void* first_key, void* second_key);

int compare_int(void* first_key, void* second_key);

int compare_char(void* first_key, void* second_key);

int compare_long(void* first_key, void* second_key);

#endif