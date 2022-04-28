#include "card.hpp"
#include "classes.hpp"
#include "text_field.hpp"

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

log* object::l;

menu* m;
za_mapo* z;

int main(){
    initscr();

    cbreak();
    noecho();
    curs_set(0);

    start_color();

    object::l = new log(10, 50, 0, 11);
    m = new menu(3, 50, 12, 0);
    z = new za_mapo(10, 10, 0, 0);

    refresh();

    blink_cfg objs[] = {
        {new mage(3, 1, string("Maga")), {GREEN_ON}},
        {new golem(8, 8, string("Goga")), {RED_STABILE}},
        {new golem(8, 7, string("Pisos")), {RED_STABILE}},
        {new golem(7, 3, string("Ugga Boogga")), {RED_STABILE}},
        {NULL, {HIDE}}
    };

    strcpy((char*)z->mapa, test_card);
    z->objects = objs;

    z->init_palitra();
    z->update_card();

    m->newline("first option\n");
    m->newline("second option\n");
    m->newline("thirst option\n");
    m->select(0);

    char buffer[41];

    timeout(500);

    char temp = 0;
    while((temp = getch()) != ' '){
        wclear(z->win);
        switch(temp){
            case -1:
                break;

            case ',':
                m->up();
                break;

            case '.':
                m->down();
                break;

            default:
                z->move(objs[0].o, temp);
                for(int i=1; objs[i].o; i++){
                    if(objs[i].o->is_alive() && abs(objs[i].o->X, objs[0].o->X) <= 1 && abs(objs[i].o->Y, objs[0].o->Y) <= 1){
                        sprintf(buffer, "Golem %s attack %s!\n", objs[i].o->get_name(), objs[0].o->get_name());
                        object::l->newline(buffer);
                        objs[0].o->act(CRUSH_ATTACK, {1, 10});
                        object::l->print();
                    }
                    else z->magnetic_search(objs[i].o, objs[0].o);
                }
        }

        object::l->print();
        z->update_card();
        m->print();
    }

    endwin();
    return 0;
}