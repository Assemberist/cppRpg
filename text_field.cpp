#include "text_field.hpp"

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