#include <ncurses.h>
#include "log.hpp"
#include <string.h>

log::log() : strings(nullptr), count(0), current(0) {}

void log::purge(){
    if(!strings)
        return;

    this->clear();
    delete[] strings;
    strings = nullptr;
    count = 0;
}

void log::clear(){
    current = 0;
    for(int i=0; i<count; i++)
        delete[] strings[i];
}

void log::reserve(short amount){
    purge();

    strings = new char*[amount];
    count = amount;
    current = 0;
    while(amount--)
        strings[amount] = nullptr;
}

void log::print(){
    wclear(win);

    if(strings[current])
        for(int i=current; i<count; i++)
            wprintw(win, strings[i]);

    for(int i=0; i<current; i++)
        wprintw(win, strings[i]);

    wrefresh(win);
}

void log::newline(const char* src){
    if(strings[current])
        delete[] strings[current];
    
    strings[current] = new char[strlen(src)+1];
    strcpy(strings[current], src);

    current = current == count-1 ? 0 : current+1;
}

void log::connect_to_win(WINDOW* _win){ win = _win; }

log::~log(){ purge(); }