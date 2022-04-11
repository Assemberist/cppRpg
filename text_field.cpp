#include "text_field.hpp"

//--------------------------------------------------------------------------------------------//
//  Text field functions                                                                      //
//____________________________________________________________________________________________//

text_field::text_field() : strings(nullptr), count(0), current(0) {}

void text_field::reserve(short amount){
    purge();

    strings = new char*[amount];
    count = amount;
    current = 0;
    while(amount--)
        strings[amount] = nullptr;
}

void text_field::connect_to_win(WINDOW* _win){ win = _win; }

void text_field::putline(char* src){
    if(strings[current])
        delete[] strings[current];
    
    strings[current] = src;

    current = current == count-1 ? 0 : current+1;
}

void text_field::clear(){
    current = 0;
    for(int i=0; i<count; i++)
        delete[] strings[i];
}

void text_field::purge(){
    if(!strings)
        return;

    this->clear();
    delete[] strings;
    strings = nullptr;
    count = 0;
}

text_field::~text_field(){ purge(); }

//--------------------------------------------------------------------------------------------//
//   Log functions                                                                            //
//____________________________________________________________________________________________//

void log::print(){
    wclear(win);
    size_t row = current % count;

    if(strings[row])
        for(int i=row; i<count; i++)
            wprintw(win, strings[i]);

    for(int i=0; i<row; i++)
        wprintw(win, strings[i]);

    wrefresh(win);
}

void log::newline(const char* src){
    size_t row = current % count;
    if(strings[row])
        delete[] strings[row];
    
    strings[row] = new char[strlen(src)+6];
    sprintf(strings[row], "%4d %s", current+1, src);

    current++;
}

//--------------------------------------------------------------------------------------------//
//   Menu functions                                                                           //
//____________________________________________________________________________________________//

void menu::newline(const char* src){
    if(strings[current])
        delete strings[current];

    strings[current] = new char[strlen(src)+1];
    strcpy(strings[current], src);
    current++;
}

void menu::up(){
    if(current < count-1)
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