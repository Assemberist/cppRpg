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
    void** elements;
    void(*print_fun)(WINDOW*, size_t, size_t, void**, char**);

public:
    menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void print();

    void up();
    void down();

    void set_content(void** elements, size_t size, const char* lexems[]);
    void set_print_fun(void(*)(WINDOW*, size_t, size_t, void**, char**));
    void shrade_elements();

    size_t get_index();
    void* get_value();
    // bool is_current_equiped();
    // void invert_equip();
    size_t size();
    //void activate(size_t num);
    //void deactivate();
};