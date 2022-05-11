#pragma once

#include "spell.hpp"
#include <ncurses.h>
#include <string.h>

class text_field{
protected:
    char** strings;
    short count;
    short current;

public:
    WINDOW* win;

    text_field(size_t rows, size_t cols, size_t pos_y, size_t pos_x);

    void reserve(short amount);
    void connect_to_win(WINDOW* _win);
    void putline(char* src);
    void hide();
    void purge();

    virtual void clear();

    virtual void print() = 0;
    virtual void newline(const char* src) = 0;

    ~text_field();
};


class log : public text_field{
public:
    log(size_t rows, size_t cols, size_t pos_y, size_t pos_x) : text_field(rows, cols, pos_y, pos_x){}
    void print();
    void newline(const char* src);
};


class menu : public text_field{
public:
    menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x) : text_field(rows, cols, pos_y, pos_x){}
    void print();
    void newline(const char* src);
    void clear();

    void up();
    void down();
    void select(size_t num);

    const char* get_selected();
};


class spell_menu : public menu{
    spell** spells;
    size_t spell_count;

public:
    spell_menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x) : menu(rows, cols, pos_y, pos_x) {}
    void input_spells(spell** _spells, size_t _spell_count);
    spell_t get_current_spell();
    void print();
};