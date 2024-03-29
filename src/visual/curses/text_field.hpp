#pragma once

#include <cstddef>
#include <ncurses.h>
#include <string.h>
#include "common/log.hpp"

class text_field{
protected:
    uint16_t count;
    uint16_t current;
    WINDOW* win;

public:
    text_field(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void hide();
    virtual void print() = 0;

    virtual ~text_field();
};

class text_log : public text_field, public log{
    char** strings;
public:
    text_log(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void print();
    void newline(const char* src);
    void clear();

    ~text_log();
};

class file_log : public log{
    FILE* log_file;
    const char* log_name;
    size_t index;

public:
    file_log(const char* file);

    void newline(const char* src);
    void nextFile();

    ~file_log();
};

template<typename T> class menu : public text_field{
protected:
    const char* capture;
    T* content;
public:
    menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x)
        : text_field(rows+1, cols, pos_y, pos_x)
        { current = 0;
          count = 0;
          capture = nullptr;
          content = nullptr; }

    void up(){
        if(current < count-1){
            current++;
            print();
        }
    }

    void set_capture(char* src){ capture = src; }

    void down(){
        if(current){
            current--;
            print();
        }
    }

    uint16_t get_selected_index(){ return current; }
    T get_selected_value(){ return content[current]; }
    uint16_t size(){ return count; }

    void delete_content(){
        delete[] content;
        content = NULL;
        count = 0;
    }

    ~menu(){ delete_content(); }
};
