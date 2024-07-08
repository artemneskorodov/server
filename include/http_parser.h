#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_URL_SIZE 128

typedef enum{
    UNKNOWN_METHOD = -1,
    GET = 0,
    HEAD = 1,
    POST = 2,
    PUT = 3,
    DELETE = 4,
    CONNECT = 5,
    OPTIONS = 6,
    TRACE = 7,
    PATCH = 8
}method_t;

typedef struct http_header{
    char *name;
    char *value;
    struct http_header *next;
}http_header_t;

typedef struct http_request{
    method_t method;
    char *url;
    http_header_t *first_header;
    char *body;
}http_request_t;


http_request_t *parse_http(char *string_request);

char *get_request_header(http_request_t *request, char *name);

int free_http_request(http_request_t *request);


#endif