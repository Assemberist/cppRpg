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
    auto* value = obj->stat.get_property(STRENGTH);
    if(value){
        target->stat.act(CRUSH_ATTACK, {0, *value});
        return;
    }
    else{
        char arr[50];
        sprintf(arr, "%s can't punch.\n", obj->get_name());
        log_msg(object::l, arr);
    }
}

void act_lighting(object* obj, object* target){
    if(obj->stat.request_property(MANA, 10)){
        target->stat.act(ELECTRIC_DAMAGE, {0, 40});
        target->stat.act(MAGIC_ATTACK, {0, 10});
    }
    else{
        log_msg(object::l, "Not enough mana.\n");
    }
}

// todo
void act_fireball(object* obj, object* target){
    
}