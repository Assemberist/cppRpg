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

int main(){
    init_graphic();

    object* objs[] = {
        new mage(3, 1, string("Maga")),
        new golem(8, 8, string("Goga")),
        new golem(8, 7, string("Pisos")),
        new golem(7, 3, string("Ugga Boogga")),
        NULL
    };

    state stat;
    objs[0]->inventory.push_back({{ARMORY_HEAD, LATEX_HELM}, NULL, stat});
    objs[0]->inventory.push_back({{ARMORY_HAND, LATEX_GLOVE}, NULL, stat});

    for(object** i = objs+1; *i; i++)
        (*i)->graph_state = RED_STABILE;

    screen s((char*)test_card, (drawable_object**)objs);

    if(!game_loop(objs, *objs, s)){

        s.common_log->newline("Game ower\n");
        s.common_log->print();

        getch();
    }

    return 0;
}