#include "card.hpp"

void init_palitra(){
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
}


void drow_card(char (*card)[12], object** objects, size_t obj_num){
    while(obj_num--)
        card[objects[obj_num]->Y]
            [objects[obj_num]->X] = 
            objects[obj_num]->get_type();
}

void update_card(WINDOW* win, char (*card)[12], size_t rows, object** objects, size_t obj_num){
    wattron(win, COLOR_PAIR(3));
    for(int i = 0; i<rows; i++)
        wprintw(win, card[i]);
        wattroff(win, 3);    

    int color;
    for(int i = 0; i<obj_num; i++){
        (objects[i]->Y, objects[i]->X);
        switch(objects[i]->get_fraction()){
            case HUMANITY:
                color = 2; break;
            case MONSTER:
                color = 1; break;
            default:
                color = 3; break;
        }
        wattron(win, COLOR_PAIR(color));
        wmove(win, objects[i]->Y, objects[i]->X);
        waddch(win, objects[i]->get_type());
        wattroff(win, color);
    }
    //refresh();
    wrefresh(win);
}