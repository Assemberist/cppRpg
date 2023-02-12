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

void menu::print(){
    wclear(win);
    print_fun(win, count, current, elements, strings);
    wrefresh(win);
}

/*void menu::shrade_elements(){
    delete[] elements;
    elements = NULL;
    strings = NULL;
}*/

void menu::set_content(void** bag, size_t size, const char* lexems[]){
    strings = (char**)lexems;
    elements = bag;
    count = size;
    current = 0;
}

void menu::up(){ if(current < count-1) current++; }
void menu::down(){ if(current) current--; }
void menu::set_print_fun(void(*fun)(WINDOW*, size_t, size_t, void**, char**)){ print_fun = fun; }
void menu::shrade_elements(){
    while(count--) delete (char*)elements[count]; // char* not calls destructor. 
    delete[] elements;
    count = 0;
}

size_t menu::get_index(){ return current; }
void* menu::get_value(){ return elements[current]; }
size_t menu::size(){ return count;}

//void menu::activate(size_t num){ isActive = true; current = num >= count ? count-1 : num; }
//void menu::deactivate(){ isActive = false; }

// bool inventory::is_current_equiped(){ return bag[current].is_equiped; }
// void inventory::invert_equip(){ bag[current].is_equiped = !bag[current].is_equiped; }
