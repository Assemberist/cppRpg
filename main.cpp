#include "card.hpp"
#include "classes.hpp"
#include "pathfinder.hpp"
#include "log.hpp"

#define abs(A, B) ((A) - (B) > 0 ? (A) - (B) : (B) - (A))

#define ROWS 10
#define COLS 10

char my_card[10][12] = {
    {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '\0'},
    {'*', '*', '*', ' ', ' ', '*', ' ', ' ', ' ', '*', '\0'},
    {'*', '*', '*', ' ', '*', '*', ' ', ' ', ' ', '*', '\0'},
    {'*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '\0'},
    {'*', '*', '*', ' ', ' ', '*', ' ', ' ', ' ', '*', '\0'},
    {'*', '*', '*', ' ', '*', '*', '*', '*', '*', '*', '\0'},
    {'*', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '\0'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '\0'},
    {'*', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '\0'},
    {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '\0'},
};

int main(){
    initscr();

    cbreak();
    noecho();

    object* objs[4] = {
        new mage(3, 1, string("Maga")),
        new golem(8, 8, string("Goga")),
        new golem(8, 7, string("Pisos")),
        new golem(7, 3, string("Ugga Sukka"))
    };

    /*WINDOW* win = newwin(10, 10, 0, 0);
    WINDOW* log = newwin(10, 30, 0, 11);

    
    drow_card(my_card, objs, 4);
    wclear(win);
    update_card(win, my_card, ROWS);
    refresh();

    char temp = 0;
    do{
        wclear(win);
        move(my_card, objs[0], temp);
        for(int i=4; --i;){
            magnetic_search(my_card, objs[i], objs[0]);
            if(abs(objs[i]->X, objs[0]->X) <= 1 && abs(objs[i]->Y, objs[0]->Y) <= 1){
                objs[0]->act({CRUSH_ATTACK, {1, 10}});
                wprintw(log, "Golem attack the mage!\n");
                wrefresh(log);
            }
        }
        update_card(win, my_card, ROWS);
    }
    while((temp = getch()) != ' ');

    */

    WINDOW* _log = newwin(10, 30, 0, 0);

    log l;
    l.reserve(10);
    l.connect_to_win(_log);

    short i = 1;
    char sas[30];
    char c;

    while((c = getch()) != ' '){
        sprintf(sas, "%4dth symbol is: %c\n", i, c);
        l.newline(sas);
        l.print();
        i++;
    }

    delwin(_log);

    endwin();
    return 0;
}