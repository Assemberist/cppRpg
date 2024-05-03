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
    objs[0]->inventory.push_back({ARMORY_HEAD, LATEX_HELM, NULL, stat});
    objs[0]->inventory.push_back({ARMORY_HAND, LATEX_GLOVE, NULL, stat});
    objs[0]->inventory.push_back({ARMORY_HAND, LATEX_GLOVE, NULL, stat});
    objs[0]->inventory.push_back({ARMORY_HAND, LATEX_GLOVE, NULL, stat});

    for(object** i = objs+1; *i; i++)
        (*i)->graph_state = RED_STABILE;

    screen s((char*)test_card, (drawable_object**)objs);

    vector<object_group*> partyes;
    object_group groups[3];
    // global
    groups[0].set_fraction(GLOBAL);
    for(object** i = objs+1; *i; i++)
        groups[0].add_object(*i);

    // mage group
    groups[1].set_fraction(HUMANITY);
    groups[1].add_object(objs[0]);
    objs[0]->set_group_membership(groups + 1);
    // golems
    groups[2].set_fraction(MONSTER);
    groups[2].add_object(objs[1]);
    objs[1]->set_group_membership(groups + 2);
    groups[2].add_object(objs[2]);
    objs[2]->set_group_membership(groups + 2);
    groups[2].add_object(objs[3]);
    objs[3]->set_group_membership(groups + 2);

    for(int i = 3; i--;) partyes.push_back(groups + i);

    if(!game_loop(objs, *objs, s)){

        s.common_log->newline("Game ower\n");
        s.common_log->print();

        getch();
    }

    return 0;
}