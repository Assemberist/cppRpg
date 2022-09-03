#include "standard_lexems.h"

lexem lexems[lexem_number] = {
    {"?", IF},
    {"else", ELSE},
    {"???", EXIST},
    {">=", LARGE_OR_EQ},
    {"<=", LESS_OR_EQ},
    {"=", EQ},
    {"!=", NO_EQ},
    {"<", LESS},
    {"->", THEN},
    {"exit", EXIT},
    {"    ", TAB},
    {"\t", TAB}
};

static size_t NonStandardAmount = 0;
static char* buffer;

token* init_parser(){

}

lexem* next_lexem(char* src){

}