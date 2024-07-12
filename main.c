#include "./include/server.h"
#include "functions.c"

int main(void){
    hashmap_t *functions = create_functions_hashmap();
    if(functions == NULL){
        printf("Hashmap init error\n");
        exit(-1);
    }

    http_server_t *server = create_http_server(8, "0.0.0.0", 3000, functions);
    if(server == NULL){
        printf("Server init error\n");
        hashmap_free(functions);
        exit(-1);
    }


    while(!0){
        char buffer[128] = {0, };
        printf("\033[1;33m$\033[1;95madmin: \033[0m");
        int scanf_res = scanf("%s", buffer);
        if(scanf_res <= 0 || scanf_res >= 127){
            printf("error reading\r\n");
            continue;
        } 

        if(strcmp(buffer, "stop") == 0){

            stop_http_server(server);
            hashmap_free(functions);
            exit(0);

        }
    }
}