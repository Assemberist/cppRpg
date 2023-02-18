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

template<typename T> class menu : public text_field{
    T* content;
public:
    menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void up();
    void down();

    short get_selected_index();
    T get_selected_value();
    short size();

    void delete_content();

    ~menu();
};