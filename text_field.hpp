#pragma once

#include <ncurses.h>
#include <string.h>

class text_field{
protected:
    short count;
    short current;
    WINDOW* win;

public:
    text_field(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void hide();
    virtual void print() = 0;

    ~text_field();
};


class log : public text_field{
    char** strings;

public:
    log(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void print();
    void newline(const char* src);
    void clear();

    ~log();
};

struct menu_element{
    const char* name;
    void* element;
};

class menu : public text_field{
    menu_element** elements;

public:
    menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void print();

    void up();
    void down();

    void set_content(menu_element** elements, size_t size);
    void* get_selected();
};
