#include "../include/http_parser.h"



int free_request_headers(http_header_t *first_header);



method_t get_request_method(char *req){
    if(req[0] == 'G' && req[1] == 'E' && req[2] == 'T') return GET;
    if(req[0] == 'H' && req[1] == 'E' && req[2] == 'A' && req[3] == 'D') return HEAD;
    if(req[0] == 'P' && req[1] == 'O' && req[2] == 'S' && req[3] == 'T') return POST;
    if(req[0] == 'P' && req[1] == 'U' && req[2] == 'T') return PUT;
    if(req[0] == 'D' && req[1] == 'E' && req[2] == 'L' && req[3] == 'E' && req[4] == 'T' && req[5] == 'E') return DELETE;
    if(req[0] == 'C' && req[1] == 'O' && req[2] == 'N' && req[3] == 'N' && req[4] == 'E' && req[5] == 'C' && req[6] == 'T') return CONNECT;
    if(req[0] == 'O' && req[1] == 'P' && req[2] == 'T' && req[3] == 'I' && req[4] == 'O' && req[5] == 'N' && req[6] == 'S') return OPTIONS;
    if(req[0] == 'P' && req[1] == 'A' && req[2] == 'T' && req[3] == 'C' && req[4] == 'H') return PATCH;
    if(req[0] == 'T' && req[1] == 'R' && req[2] == 'A' && req[3] == 'C' && req[4] == 'E') return TRACE;

    return UNKNOWN_METHOD;
}


http_request_t *parse_http(char *string_request){
    http_request_t *request = (http_request_t *)malloc(sizeof(http_request_t));

    if(request == NULL) return NULL;

    request->first_header = NULL;

    request->method = get_request_method(string_request);

    if(request->method == UNKNOWN_METHOD){
        
        free(request);
        return NULL;

    }

    size_t index = 0;

    for(; string_request[index] != ' '; index++){

        if(string_request[index] == 0){

            free(request);
            return NULL;

        }

    }
    index++;
    //index is now on first letter of url;

    size_t url_size = 0;
    for(; string_request[index + url_size] != ' '; url_size++){

        if(string_request[index] == '\n' || string_request[index] == '\r' || string_request[index] == 0){
            free(request);
            return NULL;
        }

    }

    request->url = (char *)malloc(sizeof(char) * (url_size + 1));

    if(request->url == NULL){

        free(request);
        return NULL;

    }

    request->url[url_size] = 0;

    for(size_t j = 0; j < url_size; index++, j++) request->url[j] = string_request[index];

    for(; string_request[index] != '\r'; index++){

        if(string_request[index] == 0){

            free(request->url);
            free(request);
            return NULL;

        }

    }

    index += 2;//string_request[index] now points to first letter of first header


    /*getting headers*/
    while(string_request[index - 2] != '\r' || string_request[index - 1] != '\n' || string_request[index] != '\r' || string_request[index + 1] != '\n'){

        http_header_t *header = (http_header_t *)malloc(sizeof(http_header_t));

        if(header == NULL){

            printf("was here1");
            
            free_request_headers(request->first_header);
            free(request->url);
            free(request);
            return NULL;

        }


        /*getting header name*/
        size_t header_name_size = 0;

        for(; string_request[index + header_name_size] != ':'; header_name_size++){

            if(string_request[index + header_name_size] == 0){

                printf("was here2 %lld\n", header_name_size);

                free_request_headers(request->first_header);
                free(request->url);
                free(request);
                free(header);
                return NULL;

            }

        }

        header->name = (char *)malloc(sizeof(char) * (header_name_size + 1));

        if(header->name == NULL){

            printf("was here4 %lld\n", header_name_size);
            
            free_request_headers(request->first_header);
            free(request->url);
            free(header);
            free(request);
            return NULL;

        }

        header->name[header_name_size] = 0;

        for(size_t j = 0; j < header_name_size; j++, index++) header->name[j] = string_request[index];

        index += 2;//string_request[index] now points to first letter of header value


        size_t header_value_size = 0;

        for(; string_request[index + header_value_size] != '\r'; header_value_size++){

            if(string_request[index + header_value_size] == 0){

                printf("was here3 %lld\n", header_value_size);
                
                free_request_headers(request->first_header);
                free(request->url);
                free(header->name);
                free(request);
                free(header);
                return NULL;

            }

        }

        header->value = (char *)malloc(sizeof(char) * (header_value_size + 1));

        if(header->value == NULL){

            printf("was here5 %lld\n", header_value_size);

            free_request_headers(request->first_header);
            free(request->url);
            free(header->name);
            free(header);
            free(request);
            return NULL;

        }

        header->value[header_value_size] = 0;

        for(size_t j = 0; j < header_value_size; j++, index++) header->value[j] = string_request[index];

        index += 2;

        header->next = request->first_header;
        request->first_header = header;

    }

    index += 2;

    size_t body_size = 0;

    for(; string_request[index + body_size] != 0; body_size++);

    request->body = (char *)malloc(sizeof(char) * (body_size + 1));

    if(request->body == NULL){

        free_request_headers(request->first_header);
        free(request->url);
        free(request);
        return NULL;

    }

    request->body[body_size] = 0;

    for(size_t j = 0; j < body_size; j++, index++) request->body[j] = string_request[index];

    return request;
    
}



int free_request_headers(http_header_t *first_header){

    if(first_header == NULL) return 0;

    http_header_t *next = first_header->next;

    free(first_header->name);
    free(first_header->value);
    free(first_header);

    return free_request_headers(next);

}



char *get_request_header(http_request_t *request, char *name){

    http_header_t *current = request->first_header;

    while(current != NULL){

        if(strcmp(current->name, name) == 0) return current->value;

        current = current->next;

    }

    return NULL;

}



int free_http_request(http_request_t *request){
    free_request_headers(request->first_header);
    free(request->url);
    free(request->body);
    free(request);
    return 0;
}