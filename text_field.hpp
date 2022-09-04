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

struct menu_element{
    int name;
    void* element;
};

class menu : public text_field{
    menu_element* elements;

public:
    menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void print();

    void up();
    void down();

    void set_content(menu_element* elements, size_t size, const char* lexems[]);
    int get_selected_key();
    void* get_selected_value();
    short get_index();
    void shrade_elements();
};

struct bag_element{
    bool is_equiped :1;
    size_t type :31;
    size_t element;
};

class inventory : public text_field{
    bag_element* bag;

public:
    inventory(size_t rows, size_t cols, size_t pos_y, size_t pos_x);
    void print();

    void up();
    void down();
    
    void set_content(bag_element* elements, size_t size, const char* lexems[]);

    size_t get_selected_key();
    size_t get_selected_value();
    short get_index();
    void shrade_elements();
};