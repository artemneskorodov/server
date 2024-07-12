#include "../include/server.h"

void *worker(void *args);


struct http_server{
    socket_t server_socket;
    pthread_mutex_t mutex;
    pthread_t *threads;
    hashmap_t *functions;
    int threads_number;
    int active_flag;
};

http_server_t *create_http_server(int threads_number, char *ip, int port, hashmap_t *functions){

    http_server_t *server = malloc(sizeof(http_server_t));
    if(server == NULL) return NULL;


    server->server_socket = create_tcp(ip, port);
    if(server->server_socket < 0){
        free(server);
        return NULL;
    }


    server->threads_number = threads_number;
    if(pthread_mutex_init(&server->mutex, NULL) != 0){
        close_tcp(server->server_socket);
        free(server);
        return NULL;
    }


    server->threads = malloc(sizeof(pthread_t) * threads_number);

    if(server->threads == NULL){
        close_tcp(server->server_socket);
        pthread_mutex_destroy(&server->mutex);
        free(server);
        return NULL;
    }


    server->active_flag = 1;
    server->functions = functions;


    for(int i = 0; i < threads_number; i++){

        if(pthread_create(server->threads + i, NULL, &worker, (void *)server) != 0){

            close_tcp(server->server_socket);

            server->active_flag = 0;

            for(int j = 0; j < i; j++){

                pthread_join(server->threads[j], NULL);

            }

            pthread_mutex_destroy(&server->mutex);
            free(server->threads);
            free(server);
            return NULL;

        }

    }


    return server;


}



int stop_http_server(http_server_t *server){

    server->active_flag = 0;

    for(int i = 0; i < server->threads_number; i++) pthread_join(server->threads[i], NULL);

    close_tcp(server->server_socket);

    pthread_mutex_destroy(&server->mutex);
    free(server->threads);
    free(server);
    return 0;
}



void *worker(void *args){
    http_server_t *server = (http_server_t *)args;

    if(server == NULL) pthread_exit(NULL);


    while(!0){
        if(server->active_flag == 0) pthread_exit(NULL);

        pthread_mutex_lock(&server->mutex);
        socket_t accepted = accept_tcp(server->server_socket);
        pthread_mutex_unlock(&server->mutex);

        if(accepted < 0) continue;

        size_t recv_size = 0;
        char *request_string = recv_tcp(accepted, &recv_size);
        if(request_string == NULL){
            close_tcp(accepted);
            continue;
        }

        http_request_t *request = parse_http(request_string);
        
        free(request_string);

        if(request == NULL){
            free_http_request(request);
            close_tcp(accepted);
            continue;
        }

        char *(*function)(http_request_t *, size_t *) = (char *(*)(http_request_t *, size_t *))hashmap_search(server->functions, request->url);
        if(function == NULL){
            free_http_request(request);


            char *response = "HTTP/1.1 404 Not Found\r\n\r\n";
            send_tcp(accepted, response, 26);


            close_tcp(accepted);
            continue;
        }




        size_t response_size = 0;
        char *response = function(request, &response_size);

        if(response == NULL){
            /*response generator error*/
            free_http_request(request);
            close_tcp(accepted);
            continue;
        }

        send_tcp(accepted, response, response_size);

        free_http_request(request);
        free(response);
        close_tcp(accepted);
    }
}
