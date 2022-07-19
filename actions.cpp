#include "actions.hpp"

void do_action(object* obj, object* target, spell_t sp){
    void (*fun)(object*, object*);

    switch(sp){
        case FIREBALL: fun = act_fireball; break;
        case PUNCH: fun = act_punch; break;
        case LIGHTING: fun = act_lighting; break;
        default: fun = do_nothing;
    }

    fun(obj, target);
}

void do_nothing(object*, object*){}

void act_punch(object* obj, object* tartget){
    auto* value = obj->stat.get_property(STRENGTH);
    if(value){
        obj->stat.act(CRUSH_ATTACK, {0, *value});
        return;
    }
    else{
        char arr[50];
        sprintf(arr, "%s can't punch.\n", obj->get_name());
        object::l->newline(arr);
    }
}

void act_lighting(object* obj, object* tartget){
    if(obj->stat.request_property(MANA, 10)){
        obj->stat.act(ELECTRIC_DAMAGE, {0, 40});
        obj->stat.act(MAGIC_ATTACK, {0, 10});
    }
    else{
        object::l->newline("Not enough mana.\n");
    }
}

// todo
void act_fireball(object* obj, object* target){
    
}