#include "../include/text_parser.h"

char *change_text(char *input, char *from, char *to, size_t *input_size){

    size_t from_size = strlen(from);
    size_t to_size = strlen(to);

    int changes_number = 0;
    for(size_t index = 0; index < *input_size - from_size; index++){

        int flag = 0;
        for(size_t i = 0; i < from_size; i++){
            if(input[index + i] != from[i]){
                flag = 1;
                break;
            }
        }

        if(flag == 0){
            changes_number++;
            index += from_size - 1;
            continue;
        }

    }

    size_t new_size = *input_size + changes_number *(to_size - from_size);

    char *output = (char *)malloc(new_size);

    if(output == NULL){
        return NULL;
    }


    for(size_t old_index = 0, new_index = 0; old_index < *input_size; old_index++, new_index++){

        int flag = 0;
        for(size_t i = 0; i < from_size; i++){
            if(input[old_index + i] != from[i]){
                flag = 1;
                break;
            }
        }


        if(flag == 0){
            for(size_t i = 0; i < to_size; i++){
                output[new_index + i] = to[i];
            }

            old_index += from_size - 1;
            new_index += to_size - 1;
            continue;
        }

        output[new_index] = input[old_index];

    }

    free(input);
    return output;
}