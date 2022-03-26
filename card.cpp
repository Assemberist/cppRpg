#include "card.hpp"

void drow_card(char (*card)[12], object** objects, size_t obj_num){
    while(obj_num--)
        card[objects[obj_num]->Y]
            [objects[obj_num]->X] = 
            objects[obj_num]->get_type();
}

void update_card(char (*card)[12], size_t rows){
    for(int i = 0; i<rows; i++)
        printw(card[i]);
    refresh();
}