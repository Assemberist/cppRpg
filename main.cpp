#include "card.hpp"
#include "classes.hpp"
#include "pathfinder.hpp"

#define abs(A, B) ((A) - (B) > 0 ? (A) - (B) : (B) - (A))

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
        new golem(8, 8, string("Goga")),
        new golem(8, 7, string("Pisos")),
        new golem(7, 3, string("Ugga Sukka"))
    };

    drow_card(my_card, objs, 4);
    update_card(my_card, ROWS);
    refresh();

    char temp = 0;
    do{
        clear();
        move(my_card, objs[0], temp);
        for(int i=4; --i;){
            magnetic_search(my_card, objs[i], objs[0]);
            if(abs(objs[i]->X, objs[0]->X) <= 1)
                objs[0]->act({CRUSH_ATTACK, 10});
        }
        update_card(my_card, ROWS);
        refresh();
    }
    while((temp = getch()) != ' ');

    endwin();
    return 0;
}