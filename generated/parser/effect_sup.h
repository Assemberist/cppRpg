#pragma once

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum item_t{
    EFF_PURE,
    EFF_PERMANENT,
    EFF_SHARED,
    EFF_PERMANENT_ANY,
    EFF_SHARED_ANY,
    EFF_PERMASHARED,
    EFF_ANY,
    EFF_PROPERTY,
    EFF_NUMBER
} item_t;

typedef struct expr_part{
    item_t mark;
    char* name;
} effect_s;

typedef struct effect_stack{
    effect_s effects[20];
    size_t current;
} effect_stack;

typedef struct main_buffer{
    char buffer[512];
    char* tail;
} main_buffer;

void effectFound(effect_stack* stack, char* name, item_t mark);
