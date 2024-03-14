#include "actions.hpp"

#ifdef DONT_LOG_ACTIONS
    #define log_msg(A, B)
#else
    #define log_msg(A, B) if((A)){(A)->newline(B);}
#endif

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

void act_punch(object* obj, object* target){
    auto* value = obj->stat.get_effect_perm({0, STRENGTH});
    if(value){
        target->act({1,CRUSH_ATTACK}, {0, *value});
        obj->exp.add(10);
        return;
    }
    else{
        char arr[50];
        sprintf(arr, "%s can't punch.\n", obj->get_name());
        object::newline(arr);
    }
}

void act_lighting(object* obj, object* target){
    if(obj->stat.request_property({0, MANA}, 10)){
        target->act({1,ELECTRIC_DAMAGE}, {0, 40});
        target->act({1,MAGIC_ATTACK}, {0, 10});
        obj->exp.add(15);
    }
    else{
        object::newline("Not enough mana.\n");
    }
}

// todo
void act_fireball(object* obj, object* target){
    
}

void act_throw(object* obj, object* target){
    if(obj == target) {
        object::newline("You throw and catch item. But why?\n");
        return;
    }
    target->act({1,CRUSH_ATTACK}, {0, 20});

    for(auto i = obj->item_to_use->stat.effects.begin(); i != obj->item_to_use->stat.effects.end(); i++)
        if(is_shared(i->first) == SHARED)
            target->act(i->first, i->second);
}
