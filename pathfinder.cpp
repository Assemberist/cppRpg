#include "pathfinder.hpp"

void magnetic_search(char (*map)[12], object* from, object* to){
    if(from->Y < to->Y){ // down
        if(from->X == to->X && move(map, from, 'x')) return; // just down
        if(from->X < to->X){ // down and right
            if(move(map, from, 'c')) return;
            if(from->X - to->X > from->Y - to->Y)
                if(move(map, from, 'd') || move(map, from, 'x'))
                    return;
            else
                if(move(map, from, 'x') || move(map, from, 'd'));
        }
        else if(from->X > to->X){ // down and left
            if(move(map, from, 'z')) return;
            if(to->X - from->X > from->Y - to->Y)
                if(move(map, from, 'a') || move(map, from, 'x'))
                    return;
            else
                if(move(map, from, 'x') || move(map, from, 'a'));
        }
    }
    else if(from->Y > to->Y){ // up
        if(from->X == to->X && move(map, from, 'w')) return; // just up
        if(from->X < to->X){ // up and right
            if(move(map, from, 'e')) return;
            if(from->X - to->X > to->Y - from->Y)
                if(move(map, from, 'd') || move(map, from, 'w'))
                    return;
            else
                if(move(map, from, 'w') || move(map, from, 'd'));
        }
        else if(from->X > to->X){ // up and left
            if(move(map, from, 'q')) return;
            if(to->X - from->X > to->Y - from->Y)
                if(move(map, from, 'a') || move(map, from, 'w'))
                    return;
            else
                if(move(map, from, 'w') || move(map, from, 'a'));
        }
    }
    else move(map, from, from->X < to->X ? 'd' : 'a');
}

char move(char (*arr)[12], object* obj, char direction){
    int8_t newX, newY;
    switch(direction){
        case 'q':
            newX = obj->X-1;
            newY = obj->Y-1;
            break;
        case 'w':
            newX = obj->X;
            newY = obj->Y-1;
            break;
        case 'e':
            newX = obj->X+1;
            newY = obj->Y-1;
            break;
        case 'd':
            newX = obj->X+1;
            newY = obj->Y;
            break;
        case 'c':
            newX = obj->X+1;
            newY = obj->Y+1;
            break;
        case 'x':
            newX = obj->X;
            newY = obj->Y+1;
            break;
        case 'z':
            newX = obj->X-1;
            newY = obj->Y+1;
            break;
        case 'a':
            newX = obj->X-1;
            newY = obj->Y;
            break;
        default:
            return '\0';
    }

    if(arr[newY][newX] == ' '){
        arr[newY][newX] = obj->get_type();
        arr[obj->Y][obj->X] = ' ';
        obj->X = newX;
        obj->Y = newY;
    }
    else direction = '\0';

    return direction;
}