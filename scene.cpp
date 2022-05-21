#include "scene.hpp"

struct npc_state{
    action_t last_action;
    blink_cfg* target;
};

void do_stand(blink_cfg* objs, screen s, npc_state* stats, size_t num);
void do_attack(blink_cfg* objs, screen s, npc_state* stats, size_t num);
void do_attack_nearlest_enemy(blink_cfg* objs, screen s, npc_state* stats, size_t num);
void do_search_enemy(blink_cfg* objs, screen s, npc_state* stats, size_t num);
void do_walk(blink_cfg* objs, screen s, npc_state* stats, size_t num);
void do_run_avay(blink_cfg* objs, screen s, npc_state* stats, size_t num);
void do_rest(blink_cfg* objs, screen s, npc_state* stats, size_t num);

void (*do_list[])(blink_cfg*, screen, npc_state*, size_t) = {
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

void game_loop(blink_cfg* objs, blink_cfg* gamer, screen s){
    size_t obj_count = 0;
    while(objs[obj_count].o) obj_count++;

    npc_state stats[obj_count];
    memset(stats, 0, obj_count * sizeof(npc_state));

    while(1)
        for(size_t i=0; i < obj_count; i++){
            s.mapa->clear();
            s.mapa->update_card();
            s.common_log->print();

            if(objs + i == gamer){
                if(!user_turn(objs+i, s))
                    return;
            }
            else{
                objs[i].o->calculate();
                size_t fun = (size_t)objs[i].o->turn();
                do_list[fun](objs, s, stats, i);
            }
        }
}

void do_stand(blink_cfg* objs, screen s, npc_state* stats, size_t num){}

void do_attack(blink_cfg* objs, screen s, npc_state* stats, size_t num){
    size_t i;
    if(stats[num].target == NULL){
        for(i=0; i<num; i++)
            if(objs[i].o->is_alive())
                if(objs[num].o->check_enemy(objs[i].o)){
                    stats[num].target = objs + i;
                    break;
                }
    }

    if(stats[num].target == NULL){
        for(i = num+1; objs[i].o; i++)
            if(objs[i].o->is_alive())
                if(objs[num].o->check_enemy(objs[i].o)){
                    stats[num].target = objs + i;
                    break;
                }
    }

    if(stats[num].target == NULL){
        objs[num].o->set_behavior(BHV_CHILL);
        return;
    }

    if(stats[num].target->o->is_alive() == false){
        stats[num].target = NULL;
        return;
    }

    if(objs[num].o->use_attack_spells(stats[num].target->o) == false)
        s.mapa->magnetic_search(objs[num].o, stats[num].target->o);
}

void do_attack_nearlest_enemy(blink_cfg* objs, screen s, npc_state* stats, size_t num){
    int i;
    if(stats[num].target == NULL){
        for(i=0; objs[i].o; i++)
            if(objs[i].o->is_alive())
                if(objs[num].o->check_enemy(objs[i].o)){
                    stats[num].target = objs + i;
                    break;
                }
    }

    while(objs[i].o)
        if( objs[i].o->is_alive() &&
            objs[num].o->check_enemy(objs[i].o) &&
            range(objs[num].o, stats[num].target->o) < range(objs[num].o, objs[i].o))
                stats[num].target = objs+i;
}

void do_search_enemy(blink_cfg* objs, screen s, npc_state* stats, size_t num){

}

void do_walk(blink_cfg* objs, screen s, npc_state* stats, size_t num){
    s.mapa->indirect_moving(objs[num].o);
}

void do_run_avay(blink_cfg* objs, screen s, npc_state* stats, size_t num){

}

void do_rest(blink_cfg* objs, screen s, npc_state* stats, size_t num){

}
