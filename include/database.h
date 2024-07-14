#ifndef DATABASE_H
#define DATABASE_H

#define MAX_LOGIN_SIZE 64
#define MAX_PASSWORD_SIZE 64
#define MAX_TOKEN_SIZE 64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *add_user(char *login, char *password);//returns token, don't forget to free token

char *get_login(char *token);

char *get_token(char *login);

int remove_user(char *login, char *password);



#endif