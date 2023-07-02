#include "text_field.hpp"
#include <cstdio>

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
//   text_log functions                                                                       //
//____________________________________________________________________________________________//

text_log::text_log(size_t rows, size_t cols, size_t pos_y, size_t pos_x) : text_field(rows, cols, pos_y, pos_x){
    strings = new char*[rows];
    while(rows--)
        strings[rows] = NULL;
}

void text_log::clear(){
    for(size_t i=0; i<count; i++){
        if(strings[i]) delete[] strings[i];
        strings[i] = NULL;
    }
    current = 0;
}

void text_log::print(){
    wclear(win);
    size_t row = current % count;

    if(strings[row])
        for(int i=row; i<count; i++)
            wprintw(win, strings[i]);

    for(size_t i=0; i<row; i++)
        wprintw(win, strings[i]);

    wrefresh(win);
}

void text_log::newline(const char* src){
    size_t row = current % count;
    if(strings[row])
        delete[] strings[row];
    
    strings[row] = new char[strlen(src)+6];
    sprintf(strings[row], "%4d %s", current+1, src);

    current++;
}

text_log::~text_log(){
    clear();
    if(strings) delete[] strings;
    strings = NULL;
}

//--------------------------------------------------------------------------------------------//
//   text_log functions                                                                       //
//____________________________________________________________________________________________//

file_log::file_log(const char* file){
    log_name = file;
    index = 0;
    log_file = NULL;
    nextFile();
}

void file_log::newline(const char* src){ fwrite(src, 1, strlen(src), log_file); }

void file_log::nextFile(){
    if(log_file)
        fclose(log_file);

    char buffer[strlen(log_name)+9];
    sprintf(buffer, "%s_%ld.txt", log_name, index);
    log_file = fopen(buffer, "w");
    index = index == 999 ? 0 : index + 1;
}

file_log::~file_log(){
    fclose(log_file);
}
