#ifndef SERVER_H
#define SERVER_H

#include "tcp.h"
#include "hashmap.h"
#include "http_parser.h"
#include <pthread.h>

typedef struct http_server http_server_t;

http_server_t *create_http_server(int threads_number, char *ip, int port, hashmap_t *functions);

int stop_http_server(http_server_t *server);

#endif