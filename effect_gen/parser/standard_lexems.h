#pragma once

#define lexem_number 10

enum lexem_t{
    IF,
    ELSE,
    EXIST,
    LARGE_OR_EQ,
    LESS_OR_EQ,
    EQ,
    NO_EQ,
    LESS,
    THEN,
    EXIT,
    EFFECT,
    NEW_EFFECT,
    PROPERTY
};

struct lexem{
    const char* text;
    lexem_t type;
}

extern lexem lexems[lexem_number];