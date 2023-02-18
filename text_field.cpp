#include "text_field.hpp"

//--------------------------------------------------------------------------------------------//
//  Text field functions                                                                      //
//____________________________________________________________________________________________//

text_field::text_field(size_t rows, size_t cols, size_t pos_y, size_t pos_x) {
    win = newwin(rows, cols, pos_y, pos_x);
    count = rows;
    current = 0;
}

void text_field::hide(){
    wclear(win);
    wrefresh(win);
}

text_field::~text_field(){ delwin(win); }

//--------------------------------------------------------------------------------------------//
//   Log functions                                                                            //
//____________________________________________________________________________________________//

log::log(size_t rows, size_t cols, size_t pos_y, size_t pos_x) : text_field(rows, cols, pos_y, pos_x){
    strings = new char*[rows];
    while(rows--)
        strings[rows] = NULL;
}

void log::clear(){
    for(size_t i=0; i<count; i++){
        if(strings[i]) delete[] strings[i];
        strings[i] = NULL;
    }
    current = 0;
}

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

log::~log(){
    clear();
    if(strings) delete[] strings;
    strings = NULL;
}

//--------------------------------------------------------------------------------------------//
//   Menu functions                                                                           //
//____________________________________________________________________________________________//

template<typename T> menu<T>::menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x) : text_field(rows, cols, pos_y, pos_x){
    current = 0;
    count = 0;
    content = nullptr;
}

template<typename T> void menu<T>::up(){ if(current < count-1) current++; }
template<typename T> void menu<T>::down(){ if(current) current--; }

template<typename T> short menu<T>::get_selected_index(){ return current; }
template<typename T> T* menu<T>::get_selected_value(){ return content[current]; }
template<typename T> short menu<T>::size(){ return count; }

template<typename T> void menu<T>::delete_content(){
    delete[] content;
    content = NULL;
}

template<typename T> menu<T>::~menu(){ delete_content(); }
