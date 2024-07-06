#include "../include/binary_tree.h"

struct binary_tree{
    struct node_binary_tree* root;
    int (*compare_function)(void* first_key, void* second_key);
};


struct node_binary_tree{
    void *key;
    void *value;
    struct node_binary_tree* left;
    struct node_binary_tree* right;
};



struct binary_tree* binary_tree_init(int (*compare_function)(void* first_key, void* second_key)){

    if(compare_function == NULL) return NULL;

    struct binary_tree* tree = (struct binary_tree*)malloc(sizeof(struct binary_tree));

    if(tree == NULL) return NULL;

    tree->root = NULL;
    tree->compare_function = compare_function;
    return tree;
}



int insert_binary_tree(struct binary_tree* tree, void* key, void* data){
    
    if(tree == NULL) return -1;
    if(key == NULL) return -2;
    if(data == NULL) return -3;

    struct node_binary_tree* node = (struct node_binary_tree*)malloc(sizeof(struct node_binary_tree));
    if(node == NULL) return -4;

    node->key = key;
    node->value = data;
    node->right = NULL;
    node->left = NULL;

    struct node_binary_tree* current_node = tree->root;

    if(current_node == NULL){
        tree->root = node;
        return 0;
    }

    while(!0){
        int compare_result = tree->compare_function(current_node->key, node->key);

        if(compare_result > 0){
            if(current_node->right == NULL){
                current_node->right = node;
                return 0;
            }

            current_node = current_node->right;
            continue;
        }

        else if(compare_result < 0){
            if(current_node->left == NULL){
                current_node->left = node;
                return 0;
            }

            current_node = current_node->left;
            continue;
        }

        else{
            free(node);
            return -5;
        }

    }

}



void* search_binary_tree(struct binary_tree* tree, void* key){
    if(tree == NULL) return NULL;
    if(key == NULL) return NULL;

    struct node_binary_tree* current_node = tree->root;

    while(!0){
        if(current_node == NULL) return NULL;

        int compare_result = tree->compare_function(current_node->key, key);

        if(compare_result > 0){
            current_node = current_node->right;
            continue;
        }

        else if(compare_result < 0){
            current_node = current_node->left;
            continue;
        }

        else return current_node->value;
    }
}



int free_nodes_binary_tree(struct node_binary_tree* root){
    if(root == NULL) return 0;

    free_nodes_binary_tree(root->left);
    free_nodes_binary_tree(root->right);
    free(root);
    return 0;
}



int free_binary_tree(struct binary_tree* tree){
    free_nodes_binary_tree(tree->root);
    free(tree);
    return 0;
}



int compare_strings(void* first_key, void* second_key){
    return strcmp((char *)first_key, (char *)second_key);
}

int compare_int(void* first_key, void* second_key){
    if(*(int *)first_key > *(int *)second_key) return 1;
    else if(*(int *)first_key < *(int *)second_key) return -1;
    else return 0;
}

int compare_char(void* first_key, void* second_key){
    if(*(char *)first_key > *(char *)second_key) return 1;
    else if(*(char *)first_key < *(char *)second_key) return -1;
    else return 0;
}

int compare_long(void* first_key, void* second_key){
    if(*(long *)first_key > *(long *)second_key) return 1;
    else if(*(long *)first_key < *(long *)second_key) return -1;
    else return 0;
}