#pragma once

class log{
    char** strings;
    short count;
    short current;

public:
    WINDOW* win;

    log();
    void reserve(short amount);
    void print();
    void connect_to_win(WINDOW* _win);
    void newline(const char* src);
    void clear();
    void purge();
    ~log();
};