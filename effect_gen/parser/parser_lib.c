#include "parser_lib.h"

#define TMP_FILE "file.tmp"

void add_effect_ptrs(FILE* file, token* tok){
    char buffer[] = "map<effect_t, effect>::iterator prnn;\0";
    char* ptr = buffer + 34;
    while(tok->next){
        sprintf(ptr, "%d", tok->number);
        fputs(file, buffer);
    }
}

void add_property_ptrs(FILE* file, token* tok){
    char buffer[] = "map<property_t, int32_t>::iterator prnn;\0";
    char* ptr = buffer + 37;
    while(tok->next){
        sprintf(ptr, "%d", tok->number);
        fputs(file, buffer);
    }
}

token* new_token(){
    token* tok = (token*)malloc(sizeof(token));
    *tok = {NULL, NULL, NULL};
    return tok;
}

size_t check_level(char* src){
    size_t count = 0;
    size_t spaces = 0;
    while(1){
        switch(*src){
            case '\t': count++; break;
            case ' ': spaces++; break;
            default:
                return count + (spaces >> 2);
        }
        src++;
    }
}