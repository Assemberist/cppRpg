#pragma once

#include <ncurses.h>
#include <string.h>

class text_field{
protected:
    char** strings;
    short count;
    short current;

public:
    WINDOW* win;

    text_field();

    void reserve(short amount);
    void connect_to_win(WINDOW* _win);
    void putline(char* src);
    void clear();
    void purge();

    virtual void print() = 0;
    virtual void newline(const char* src) = 0;

    ~text_field();
};