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

menu::menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x) : text_field(rows, cols, pos_y, pos_x){
    current = 0;
    count = 0;
}

void menu::up(){
    if(current < count-1)
        current++;
}

void menu::down(){
    if(current)
        current--;
}

void menu::print(){
    wclear(win);
    for(int i = 0; i<count; i++){
        waddch(win, i == current ? '*' : ' ');
        wprintw(win, strings[elements[i].name]);
    }
    wrefresh(win);
}

void menu::set_content(menu_element* elements, size_t size, const char* lexems[]){
    strings = (char**)lexems;
    this->elements = elements;
    count = size;
    current = 0;
}

int menu::get_selected_key(){ return elements[current].name; }
void* menu::get_selected_value(){ return elements[current].element; }
short menu::get_index(){ return current; }