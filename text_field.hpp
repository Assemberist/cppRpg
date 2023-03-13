#pragma once

#include <cstddef>
#include <ncurses.h>
#include <string.h>

class text_field{
protected:
    uint16_t count;
    uint16_t current;
    char** strings;
    WINDOW* win;

public:
    text_field(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void hide();
    virtual void print() = 0;

    virtual ~text_field();
};


class log{
public:
    virtual void newline(const char* src) = 0;
};

class text_log : public text_field, public log{
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
    T* content;
public:
    menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x)
        : text_field(rows, cols, pos_y, pos_x)
        { current = 0;
          count = 0;
          content = nullptr; }

    void up(){
        if(current < count-1){
            current++;
            print();
        }
    }
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
