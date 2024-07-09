#ifndef TCP_H
#define TCP_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>


typedef int socket_t;


#define DEFAULT_RECV_SIZE 128
#define MAX_RECV_SIZE 2048

socket_t create_tcp(char *ip, int port);

int send_tcp(socket_t conn, char *input, size_t size);

char *recv_tcp(socket_t conn, size_t *size);//don't forget to free result

int close_tcp(socket_t conn);

socket_t accept_tcp(socket_t server_socket);

#endif