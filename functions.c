#include "./include/hashmap.h"
#include "./include/http_parser.h"
#include "./include/text_parser.h"

typedef enum{
    TEXT_HTML = 0,
    TEXT_CSS = 1,
    TEXT_JAVASCRIPT = 2,
    
}content_type_t;

hashmap_t *create_functions_hashmap(void);
char *render_static(char *path, size_t *size, content_type_t content_type);

char* render_index_html(http_request_t *request, size_t *size);
char *render_index_css(http_request_t *request, size_t *size);




/*Functions definition*/

hashmap_t *create_functions_hashmap(void){
    hashmap_t *functions = hashmap_create(128);

    int state = 0;

    state += hashmap_insert(functions, "/", &render_index_html);
    state += hashmap_insert(functions, "/css/index.css", &render_index_css);


    if(state != 0){
        hashmap_free(functions);
        return NULL;
    }

    return functions;
}


char *render_static(char *path, size_t *size, content_type_t content_type){

    FILE *file = fopen(path, "rb");

    if(file == NULL){
        *size = 0;
        return NULL;
    }

    fseek(file, 0, 2);
    size_t file_size = ftell(file);
    fseek(file, 0, 0);

    char size_string[32] = {0, };
    sprintf(size_string, "%lu", file_size);



    *size = file_size + 256;


    char *result = malloc(*size);
    if(result == NULL){
        fclose(file);
        *size = 0;
        return NULL;
    }


    for(size_t i = 0; i < *size; i++) result[i] = 0;

    strcat(result, "HTTP/1.1 200 OK\r\nContent-type: ");
    if(content_type == TEXT_HTML) strcat(result, "text/html; charset=utf-8\r\n");
    else if(content_type == TEXT_CSS) strcat(result, "text/css; charset=utf-8\r\n");
    else if(content_type == TEXT_JAVASCRIPT) strcat(result, "text/javascript; charset=utf-8\r\n");

    strcat(result, "Content-length: ");
    strcat(result, size_string);
    strcat(result, "\r\n");





    strcat(result, "\r\n");


    size_t index = strlen(result);


    if(fread(result + index, sizeof(char), file_size / sizeof(char), file) != file_size){
        free(result);
        fclose(file);
        return NULL;
    }
    fclose(file);
    return result;

}



char *render_index_html(http_request_t *request, size_t *size){
    if(request->method == GET){
        char *result = render_static("./public/index.html", size, TEXT_HTML);
        return result;
    }

    else{
        *size = 0;
        return NULL;
    }
}



char *render_index_css(http_request_t *request, size_t *size){
    if(request->method == GET){
        char *result = render_static("./public/css/index.css", size, TEXT_CSS);
        return result;
    }

    else{
        *size = 0;
        return NULL;
    }
}
