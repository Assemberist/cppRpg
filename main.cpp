#include "card.hpp"
#include "classes.hpp"
#include "text_field.hpp"
#include "user_ifc.hpp"

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

spell_menu* m;
za_mapo* z;

int main(){
    initscr();

    cbreak();
    noecho();
    curs_set(0);

    start_color();

    object::l = new log(10, 50, 0, 11);
    m = new spell_menu(3, 50, 12, 0);
    z = new za_mapo(10, 10, 0, 0);

    refresh();

    blink_cfg objs[] = {
        {new mage(3, 1, string("Maga")), {GREEN_STABILE}},
        {new golem(8, 8, string("Goga")), {RED_STABILE}},
        {new golem(8, 7, string("Pisos")), {RED_STABILE}},
        {new golem(7, 3, string("Ugga Boogga")), {RED_STABILE}},
        {NULL, {HIDE}}
    };

    strcpy((char*)z->mapa, test_card);
    z->objects = objs;

    z->init_palitra();
    z->update_card();

    screen s;
    s.common_log = object::l;
    s.common_menu = m;
    s.mapa = z;

    char buffer[41];

    timeout(500);

    while(1)
        for(int i=0; i < sizeof(objs) / sizeof(blink_cfg) - 1; i++)
            if(!user_turn(objs+i, s))
                goto sos;

sos:
    endwin();
    return 0;
}