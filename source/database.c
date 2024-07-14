#include "../include/database.h"

struct user_t{
    char login[MAX_LOGIN_SIZE + 1];
    char token[MAX_TOKEN_SIZE + 1];
    char password[MAX_PASSWORD_SIZE + 1]
};

char *alphabet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void token_init(char *output){
    for(int i = 0; i < MAX_TOKEN_SIZE; i++){
        output[i] = alphabet[rand() % 62];
    }

    FILE *file = fopen("./database/users", "r");
    struct user_t current;
    memset(&current, 0, sizeof(current));

    while(fread(&current, sizeof(char), sizeof(struct user_t) + 3, file) != 0){
        if(strcmp(output, current.token) == 0){
            for(int i = 0; i < MAX_TOKEN_SIZE; i++){
                output[i] = alphabet[rand() % 62];
            }
            fseek(file, 0, 0);
        }
        memset(&current, 0, sizeof(current));
    }
    fclose(file);
    return;
}

char *add_user(char *login, char *password){
    if(login == NULL || password == NULL) return NULL;

    size_t login_size = strlen(login), password_size = strlen(password);
    if(login_size > MAX_LOGIN_SIZE || password_size > MAX_PASSWORD_SIZE) return NULL;

    char *token = (char *)malloc(MAX_TOKEN_SIZE + 1);
    token[MAX_TOKEN_SIZE] = 0;

    token_init(token);

    FILE *file = fopen("./database/users", "a+");
    int flag = 0;
    for(int i = 0; i < MAX_LOGIN_SIZE + 1; i++){
        if(flag != 0) fputc(0, file);
        else{
            fputc(login[i], file);
            if(login[i + 1] == 0) flag = 1;
        }
    }

    flag = 0;
    for(int i = 0; i < MAX_PASSWORD_SIZE + 1; i++){
        if(flag == 0) fputc(0, file);
        else{
            fputc(password[i], file);
            if(login[i + 1] == 0) flag = 1;
        }
    }

    flag = 0;
    for(int i = 0; i < MAX_TOKEN_SIZE + 1; i++){
        if(flag == 0) fputc(0, file);
        else{
            fputc(token[i], file);
            if(token[i + 1] == 0) flag = 1;
        }
    }

    return token;
}



char *get_login(char *token){
    struct user_t current;
    memset(&current, 0, sizeof(current));

    FILE *file = fopen("./database/users", "r");
    while(fread(file, sizeof(char), sizeof(current), file) != 0){
        if(strcmp(token, current.token) == 0){
            size_t login_size = strlen(current.login);
            char *login = (char *)malloc(login_size + 1);
            if(login == NULL) return NULL;

            login[login_size] = 0;

            memcpy(login, current.login, login_size);
            return login;
        }
        memset(&current, 0, sizeof(current));
    }
    return NULL;
}



char *get_token(char *login){
    struct user_t current;
    memset(&current, 0, sizeof(current));

    FILE *file = fopen("./database/users", "r");
    while(fread(file, sizeof(char), sizeof(current), file) != 0){
        if(strcmp(login, current.login) == 0){
            size_t token_size = strlen(current.token);
            char *token = (char *)malloc(token_size + 1);
            if(token == NULL) return NULL;

            token[token_size] = 0;

            memcpy(token, current.login, token_size);
            return token;
        }
        memset(&current, 0, sizeof(current));
    }
    return NULL;
}