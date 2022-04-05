#include "menu.hpp"

void menu::newline(const char* src){
    if(strings[current])
        delete strings[current];

    strings[current] = new char[strlen(src)+1];
    strcpy(strings[current], src);
    current++;
}

void menu::up(){
    if(current < count+1)
        current++;
}

void menu::down(){
    if(current)
        current--;
}

void menu::select(size_t num){
    if(num < count)
        current = num;
}

void menu::print(){
    wclear(win);
    for(int i = 0; i<count; i++){
        waddch(win, i == current ? '*' : ' ');
        wprintw(win, strings[i]);
    }
    wrefresh(win);
}