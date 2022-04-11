#include "card.hpp"
#include "classes.hpp"
#include "text_field.hpp"

#define abs(A, B) ((A) - (B) > 0 ? (A) - (B) : (B) - (A))

const char* test_card = "*************  *   **** **   ****      ****  *   **** ******* *      **        ** *      ***********";
// Card 10x10
// **********
// ***  *   *
// *** **   *
// ***      *
// ***  *   *
// *** ******
// * *      *
// *        *
// * *      *
// **********


log l;
menu m;
za_mapo<char[10][10]> z;

int main(){
    initscr();

    cbreak();
    noecho();
    curs_set(0);

    start_color();

    object* objs[] = {
        new mage(3, 1, string("Maga")),
        new golem(8, 8, string("Goga")),
        new golem(8, 7, string("Pisos")),
        new golem(7, 3, string("Ugga Boogga")),
        NULL
    };

    WINDOW* win = newwin(10, 10, 0, 0);
    WINDOW* _log = newwin(10, 50, 0, 11);
    WINDOW* _menu = newwin(3, 50, 12, 0);

    z.win = win;
    strcpy((char*)z.mapa, test_card);
    z.objects = objs;

    refresh();

    z.init_palitra();
    z.update_card();

    l.reserve(10);
    l.connect_to_win(_log);

    m.reserve(3);
    m.connect_to_win(_menu);

    m.newline("first option\n");
    m.newline("second option\n");
    m.newline("thirst option\n");
    m.select(0);

    m.print();

    char buffer[41];

    char temp = 0;
    while((temp = getch()) != ' '){
        wclear(win);
        switch(temp){
            case ',':
                m.up();
                break;

            case '.':
                m.down();
                break;

            default:
                z.move(objs[0], temp);
                for(int i=1; objs[i]; i++){
                    z.magnetic_search(objs[i], objs[0]);
                    if(objs[i]->is_alive() && abs(objs[i]->X, objs[0]->X) <= 1 && abs(objs[i]->Y, objs[0]->Y) <= 1){
                        sprintf(buffer, "Golem %s attack %s!\n", objs[i]->get_name(), objs[0]->get_name());
                        l.newline(buffer);
                        objs[0]->act(CRUSH_ATTACK, {1, 10});
                        l.print();
                    }
                }
        }
        z.update_card();
        l.print();
        m.print();
    }

    endwin();
    return 0;
}