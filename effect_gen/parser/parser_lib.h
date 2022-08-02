#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct token{
    struct token* next;
    char* token;
    size_t number;
} token;

token* new_token();

size_t check_level(char*);

// add_effect() returns NULL if can't create effect case by any reason
int add_effect(FILE*, char*);

int parse_file(FILE*, FILE*, token*, token*);
