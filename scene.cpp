#include "scene.hpp"

struct npc_state{
    action_t last_action;
    object* target;
};

void do_stand(object** objs, screen s, npc_state* stats, size_t num);
void do_attack(object** objs, screen s, npc_state* stats, size_t num);
void do_attack_nearlest_enemy(object** objs, screen s, npc_state* stats, size_t num);
void do_search_enemy(object** objs, screen s, npc_state* stats, size_t num);
void do_walk(object** objs, screen s, npc_state* stats, size_t num);
void do_run_avay(object** objs, screen s, npc_state* stats, size_t num);
void do_rest(object** objs, screen s, npc_state* stats, size_t num);

void (*do_list[])(object**, screen, npc_state*, size_t) = {
    do_stand,
    do_attack,
    do_attack_nearlest_enemy,
    do_search_enemy,
    do_walk,
    do_run_avay,
    do_rest
};

size_t range(object* obj1, object* obj2){
    size_t x = abs(obj1->X, obj2->X);
    size_t y = abs(obj1->Y, obj2->Y);
    return x*x + y*y;
}

bool game_loop(object** objs, object* gamer, screen s){
    size_t obj_count = 0;
    while(objs[obj_count]) obj_count++;

    npc_state stats[obj_count];
    memset(stats, 0, obj_count * sizeof(npc_state));

    while(1){
        if(!(gamer->is_alive()))
            return false;

        for(size_t i=0; i < obj_count; i++){
            s.mapa->clear();
            s.mapa->update_card();
            s.common_log->print();

            if(objs[i] == gamer){
                if(!user_turn(objs[i], s))
                    return true;
            }
            else{
                objs[i]->calculate();
                if(objs[i]->is_alive()){
                    size_t fun = (size_t)objs[i]->turn();
                    do_list[fun](objs, s, stats, i);
                }
            }
        }
    }
}

void do_stand(object** objs, screen s, npc_state* stats, size_t num){}

void do_attack(object** objs, screen s, npc_state* stats, size_t num){
    size_t i;
    if(stats[num].target == NULL){
        for(i=0; i<num; i++)
            if(objs[i]->is_alive())
                if(objs[num]->check_enemy(objs[i])){
                    stats[num].target = objs[i];
                    break;
                }
    }

    if(stats[num].target == NULL){
        for(i = num+1; objs[i]; i++)
            if(objs[i]->is_alive())
                if(objs[num]->check_enemy(objs[i])){
                    stats[num].target = objs[i];
                    break;
                }
    }

    if(stats[num].target == NULL){
        objs[num]->set_behavior(BHV_CHILL);
        return;
    }

    if(stats[num].target->is_alive() == false){
        stats[num].target = NULL;
        return;
    }

    if(objs[num]->use_attack_spells(stats[num].target) == false)
        s.mapa->magnetic_search(objs[num], stats[num].target);
}

void do_attack_nearlest_enemy(object** objs, screen s, npc_state* stats, size_t num){
    int i;
    if(stats[num].target == NULL){
        for(i=0; objs[i]; i++)
            if(objs[i]->is_alive())
                if(objs[num]->check_enemy(objs[i])){
                    stats[num].target = objs[i];
                    break;
                }
    }

    while(objs[i])
        if( objs[i]->is_alive() &&
            objs[num]->check_enemy(objs[i]) &&
            range(objs[num], stats[num].target) < range(objs[num], objs[i]))
                stats[num].target = objs[i];
}

void do_search_enemy(object** objs, screen s, npc_state* stats, size_t num){
    // todo
}

void do_walk(object** objs, screen s, npc_state* stats, size_t num){
    s.mapa->indirect_moving(objs[num]);
}

void do_run_avay(object** objs, screen s, npc_state* stats, size_t num){
    s.mapa->magnetic_search_neg(objs[num], stats[num].target);
}

void do_rest(object** objs, screen s, npc_state* stats, size_t num){
    // todo
}
