#include "card.hpp"
#include "classes.hpp"
#include "pathfinder.hpp"

#define ROWS 10
#define COLS 10

char my_card[10][12] = {
    {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '\n', '\0'},
    {'*', '*', '*', ' ', ' ', '*', ' ', ' ', ' ', '*', '\n', '\0'},
    {'*', '*', '*', ' ', '*', '*', ' ', ' ', ' ', '*', '\n', '\0'},
    {'*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '\n', '\0'},
    {'*', '*', '*', ' ', ' ', '*', ' ', ' ', ' ', '*', '\n', '\0'},
    {'*', '*', '*', ' ', '*', '*', '*', '*', '*', '*', '\n', '\0'},
    {'*', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '\n', '\0'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '\n', '\0'},
    {'*', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '\n', '\0'},
    {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '\n', '\0'},
};

int main(){
    initscr();

    refresh();

    object* objs[4] = {
        new mage(3, 1, string("Maga")),
        new goblin(8, 8, string("Goga")),
        new goblin(8, 7, string("Pisos")),
        new goblin(7, 3, string("Ugga Sukka"))
    };

    drow_card(my_card, objs, 4);
    update_card(my_card, ROWS);
    refresh();

    char temp = 0;
    do{
        clear();
        move(my_card, objs[0], temp);
        for(int i=4; --i; magnetic_search(my_card, objs[i], objs[0]));
        update_card(my_card, ROWS);
        refresh();
    }
    while((temp = getch()) != ' ');

    endwin();
    return 0;
}