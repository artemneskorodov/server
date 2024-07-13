#ifndef TEXT_PARSER_H
#define TEXT_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *change_text(char *input, char *from, char *to, size_t *input_size, size_t from_size, size_t to_size);

#endif