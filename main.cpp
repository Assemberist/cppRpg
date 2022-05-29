#include "classes.hpp"
#include "text_field.hpp"
#include "scene.hpp"

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

int main(){
    initscr();

    cbreak();
    noecho();
    curs_set(0);

    start_color();

    object::l = new log(10, 50, 0, 11);

    screen s;
    s.common_log = object::l;
    s.common_menu = new menu(3, 50, 12, 0);
    s.mapa = new za_mapo(10, 10, 0, 0);

    refresh();

    blink_cfg objs[] = {
        {new mage(3, 1, string("Maga")), {GREEN_STABILE}},
        {new golem(8, 8, string("Goga")), {RED_STABILE}},
        {new golem(8, 7, string("Pisos")), {RED_STABILE}},
        {new golem(7, 3, string("Ugga Boogga")), {RED_STABILE}},
        {NULL, {HIDE}}
    };

    strcpy((char*)s.mapa->mapa, test_card);
    s.mapa->objects = objs;

    s.mapa->init_palitra();
    s.mapa->update_card();

    timeout(500);

    if(!game_loop(objs, objs, s)){
        timeout(-1);
        s.common_log->newline("Game ower\n");
        s.common_log->print();
        getch();
    }

    endwin();
    return 0;
}