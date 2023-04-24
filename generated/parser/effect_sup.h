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
    EFF_NUMBER,
    EFF_THIS
} item_t;

typedef struct expr_part{
    char* name;
    item_t mark;
} effect_s;

void impl_found();
void impl_notfound();
void impl_matan();
void impl_else();
void checkTabs(size_t, size_t);

void build_expr(char* op);
void add_braces();

void write_comment(char* src);
void write_comment_and_item(char* src);

void move_comment(size_t tabs);
void put_comment();

void clean_buffers();