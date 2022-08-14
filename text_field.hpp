#pragma once

#include <ncurses.h>
#include <string.h>

class text_field{
protected:
    short count;
    short current;
    char** strings;
    WINDOW* win;

public:
    text_field(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void hide();
    virtual void print() = 0;

    ~text_field();
};


class log : public text_field{

public:
    log(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void print();
    void newline(const char* src);
    void clear();

    ~log();
};

class menu : public text_field{
    unsigned long* elements;

public:
    menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void print();

    void up();
    void down();

    void set_content(const char* str_arry[], unsigned long* content, size_t size);
    unsigned long get_selected();
};
