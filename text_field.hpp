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
protected:
    T* content;
public:
    menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x)
        : text_field(rows, cols, pos_y, pos_x)
        { current = 0;
          count = 0;
          content = nullptr; }

    void up(){ if(current < count-1) current++; }
    void down(){ if(current) current--; }

    short get_selected_index(){ return current; }
    T get_selected_value(){ return content[current]; }
    short size(){ return count; }

    void delete_content(){
        delete[] content;
        content = NULL;
        count = 0;
    }

    ~menu(){ delete_content(); }
};
