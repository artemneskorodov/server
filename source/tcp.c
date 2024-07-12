#include "../include/tcp.h"



socket_t create_tcp(char *ip, int port){

    socket_t server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket < 0) return -1;


    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 250;

    if(setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0){
        close(server_socket);
        return -5;
    }


    struct sockaddr_in server_info;

    server_info.sin_addr.s_addr = inet_addr(ip);
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(port);

    if(bind(server_socket, (struct sockaddr *)&server_info, sizeof(server_info)) != 0){
        close(server_socket);
        return -2;
    }


    if(listen(server_socket, SOMAXCONN) != 0){
        close(server_socket);
        return -3;
    }


    return server_socket;

}



int send_tcp(socket_t conn, char *input, size_t size){

    return send(conn, input, sizeof(char) * size, 0);

}



char *recv_tcp(socket_t conn, size_t *size){

    *size = DEFAULT_RECV_SIZE + 1;

    char *result = (char *)malloc((*size));

    if(result == NULL){
        *size = 0;
        return NULL;
    }


    for(size_t i = 0; i < *size; i++) result[i] = 0;


    int flag = 1;


    while(flag){
        
        int recv_state = recv(conn, result + *size - DEFAULT_RECV_SIZE - 1, DEFAULT_RECV_SIZE, 0);

        if(recv_state == -1){
            free(result);
            *size = 0;
            return NULL;
        }

        if(recv_state < DEFAULT_RECV_SIZE) flag = 0;

        *size += DEFAULT_RECV_SIZE;


        if(*size > MAX_RECV_SIZE){
            free(result);
            *size = 0;
            return NULL;
        }

        char *new = (char *)realloc(result, *size);
        if(new == NULL){
            *size = 0;
            free(result);
            return NULL;
        }

        result = new;

        for(size_t i = *size - DEFAULT_RECV_SIZE; i < *size; i++) result[i] = 0;

    }


    return result;

}



int close_tcp(socket_t conn){

    return close(conn);

}



socket_t accept_tcp(socket_t server_socket){

    struct sockaddr client;
    unsigned int size = sizeof(client);

    return (socket_t)accept(server_socket, &client, &size);

}