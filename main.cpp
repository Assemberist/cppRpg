#include "card.hpp"
#include "classes.hpp"
#include "pathfinder.hpp"
#include "log.hpp"

#define abs(A, B) ((A) - (B) > 0 ? (A) - (B) : (B) - (A))

#define ROWS 10
#define COLS 10

log l;

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
    curs_set(0);

    start_color();

    object* objs[4] = {
        new mage(3, 1, string("Maga")),
        new golem(8, 8, string("Goga")),
        new golem(8, 7, string("Pisos")),
        new golem(7, 3, string("Ugga Boogga"))
    };

    WINDOW* win = newwin(10, 10, 0, 0);
    WINDOW* _log = newwin(10, 50, 0, 11);

    refresh();

    //drow_card(my_card, objs, 4);
    init_palitra();
    update_card(win, my_card, ROWS, objs, 4);
    wrefresh(win);
    wrefresh(_log);

    l.reserve(10);
    l.connect_to_win(_log);

    char buffer[41];

    char temp = 0;
    while((temp = getch()) != ' '){
        wclear(win);
        move(my_card, objs[0], temp);
        for(int i=4; --i;){
            magnetic_search(my_card, objs[i], objs[0]);
            if(objs[i]->is_alive() && abs(objs[i]->X, objs[0]->X) <= 1 && abs(objs[i]->Y, objs[0]->Y) <= 1){
                sprintf(buffer, "Golem %s attack %s!\n", objs[i]->get_name(), objs[0]->get_name());
                l.newline(buffer);
                objs[0]->act(CRUSH_ATTACK, {1, 10});
                l.print();
            }
        }
        update_card(win, my_card, ROWS, objs, 4);
    }

    endwin();
    return 0;
}