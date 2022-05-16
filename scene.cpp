#include "scene.hpp"
#include "user_ifc.hpp"

void game_loop(blink_cfg* objs, blink_cfg* gamer, screen s){
    size_t obj_count = 0;
    while(objs[obj_count].o) obj_count++;

    while(1)
        for(int i=0; i < obj_count; i++)
            if(objs + i == gamer){
                if(!user_turn(objs+i, s))
                    return;
            }
}