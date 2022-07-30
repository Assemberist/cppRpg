#include "card.hpp"
#include <cstddef>

za_mapo::za_mapo (size_t _rows, size_t _length, size_t pos_y, size_t pos_x) {
    mapa = new char[_rows * _length + 1];
    win = newwin(_rows, _length, pos_y, pos_x);
    rows = _rows;
    length = _length;
}

void za_mapo::init_palitra () {
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    init_pair(4, COLOR_BLACK, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
}

char new_direction(int8_t* newX, int8_t* newY, char direction, object* obj){
        switch(direction){
        case 'q':
            *newX = obj->X-1;
            *newY = obj->Y-1;
            break;
        case 'w':
            *newX = obj->X;
            *newY = obj->Y-1;
            break;
        case 'e':
            *newX = obj->X+1;
            *newY = obj->Y-1;
            break;
        case 'd':
            *newX = obj->X+1;
            *newY = obj->Y;
            break;
        case 'c':
            *newX = obj->X+1;
            *newY = obj->Y+1;
            break;
        case 'x':
            *newX = obj->X;
            *newY = obj->Y+1;
            break;
        case 'z':
            *newX = obj->X-1;
            *newY = obj->Y+1;
            break;
        case 'a':
            *newX = obj->X-1;
            *newY = obj->Y;
            break;
        default:
            return '\0';
    }
    return direction;
}

void za_mapo::redraw(){
    wprintw(win, mapa);

    int color;

    for(int i = 0; objects[i]; i++){
        if(!objects[i]->is_alive() && objects[i]->graph_state != RED_INVERT)
            color = 0;

        else{
            switch(objects[i]->graph_state){
                case HIDE:
                    continue;

                case GREEN_ON:
                    color = 4;
                    objects[i]->graph_state = GREEN_OFF;
                    break;

                case GREEN_OFF:
                    color = 2;
                    objects[i]->graph_state = GREEN_ON;
                    break;

                case RED_ON:
                    color = 4;
                    objects[i]->graph_state = RED_OFF;
                    break;

                case RED_OFF:
                    color = 1;
                    objects[i]->graph_state = RED_ON;
                    break;
                
                case GREEN_STABILE:
                    color = 2;
                    break;

                case RED_STABILE:
                    color = 1;
                    break;

                case RED_INVERT:
                    color = 3;
                    break;

                case GREEN_INVERT:
                    color = 5;
                    break;
 
                default:
                    break;
            }
        }

        wattron(win, COLOR_PAIR(color));
        wmove(win, objects[i]->Y, objects[i]->X);
        waddch(win, objects[i]->get_type());
        wattroff(win, COLOR_PAIR(color));
    }
}

void za_mapo::update_card () {
    redraw();
    wrefresh(win);
}

void za_mapo::magnetic_search_neg(object* from, object* to){
    if(from->Y < to->Y){ // down
        if(from->X == to->X && move(from, 'w')) return; // just down
        if(from->X < to->X){ // down and right
            if(move(from, 'q')) return;
            if(from->X - to->X > from->Y - to->Y){
                if(move(from, 'a') || move(from, 'w'))
                    return;
            }
            else
                if(move(from, 'w') || move(from, 'a'));
        }
        else if(from->X > to->X){ // down and left
            if(move(from, 'e')) return;
            if(to->X - from->X > from->Y - to->Y){
                if(move(from, 'd') || move(from, 'w'))
                    return;
            }
            else
                if(move(from, 'w') || move(from, 'd'));
        }
    }
    else if(from->Y > to->Y){ // up
        if(from->X == to->X && move(from, 'w')) return; // just up
        if(from->X < to->X){ // up and right
            if(move(from, 'z')) return;
            if(from->X - to->X > to->Y - from->Y){
                if(move(from, 'a') || move(from, 'x'))
                    return;
            }
            else
                if(move(from, 'x') || move(from, 'a'));
        }
        else if(from->X > to->X){ // up and left
            if(move(from, 'c')) return;
            if(to->X - from->X > to->Y - from->Y){
                if(move(from, 'd') || move(from, 'x'))
                    return;
            }
            else
                if(move(from, 'x') || move(from, 'd'));
        }
    }
    else move(from, from->X < to->X ? 'a' : 'd');
}

void za_mapo::magnetic_search(object* from, object* to){
    if(from->Y < to->Y){ // down
        if(from->X == to->X && move(from, 'x')) return; // just down
        if(from->X < to->X){ // down and right
            if(move(from, 'c')) return;
            if(from->X - to->X > from->Y - to->Y){
                if(move(from, 'd') || move(from, 'x'))
                    return;
            }
            else
                if(move(from, 'x') || move(from, 'd'));
        }
        else if(from->X > to->X){ // down and left
            if(move(from, 'z')) return;
            if(to->X - from->X > from->Y - to->Y){
                if(move(from, 'a') || move(from, 'x'))
                    return;
            }
            else
                if(move(from, 'x') || move(from, 'a'));
        }
    }
    else if(from->Y > to->Y){ // up
        if(from->X == to->X && move(from, 'w')) return; // just up
        if(from->X < to->X){ // up and right
            if(move(from, 'e')) return;
            if(from->X - to->X > to->Y - from->Y){
                if(move(from, 'd') || move(from, 'w'))
                    return;
            }
            else
                if(move(from, 'w') || move(from, 'd'));
        }
        else if(from->X > to->X){ // up and left
            if(move(from, 'q')) return;
            if(to->X - from->X > to->Y - from->Y){
                if(move(from, 'a') || move(from, 'w'))
                    return;
            }
            else
                if(move(from, 'w') || move(from, 'a'));
        }
    }
    else move(from, from->X < to->X ? 'd' : 'a');
}

char za_mapo::move(object* obj, char direction){
    int8_t newX, newY;

    if(!new_direction(&newX, &newY, direction, obj)) return '\0';

    char (*ptr)[length] = (char(*)[length])mapa;

    if(ptr[newY][newX] == ' '){
        ptr[newY][newX] = obj->get_type();
        ptr[obj->Y][obj->X] = ' ';
        obj->X = newX;
        obj->Y = newY;
    }
    else direction = '\0';

    return direction;
}

void za_mapo::indirect_moving(object* from){
    move(from, "qweasdzxc"[rand() % sizeof("qweasdzxc")]);
}

void za_mapo::clear(){ wclear(win); }

za_mapo::~za_mapo(){ 
    delwin(win);
    delete[] mapa;
}

void za_mapo::free_move(object* obj, char direction){
    int8_t newX, newY;

    if(!new_direction(&newX, &newY, direction, obj)) return;

    if( newX >= 0 && newX < length &&
        newY >= 0 && newY < length){
            obj->X = newX;
            obj->Y = newY;}
}

void za_mapo::draw_range(object* target, int8_t range){
    redraw();

    int8_t fromX = target->X - range < 0 ? 0 : target->X - range+1;
    int8_t fromY = target->Y - range < 0 ? 0 : target->Y - range+1;
    int8_t toX = target->X + range > length ? length-1 : target->X + range;
    int8_t toY = target->Y + range > rows ? rows-1 : target->Y + range;

    wattron(win, COLOR_PAIR(3));
    for(int i = fromY; i < toY; i++){
        wmove(win, i, fromX);
        for(int j = fromX; j < toX; j++)
            waddch(win, ((char(*)[length])mapa)[i][j]); }

    for(int i = 0; objects[i]; i++){
        if( objects[i]->X >= fromX && objects[i]->X < toX &&
            objects[i]->Y >= fromY && objects[i]->Y < toY ){
                wmove(win, objects[i]->Y, objects[i]->X);
                waddch(win, objects[i]->get_type()); }}

    wattroff(win, COLOR_PAIR(3));

    wrefresh(win);
}

object* search_targets(object* obj, screen s, size_t range){
    static object** last_target = NULL;

    if(!last_target) last_target = s.mapa->objects;

    if(obj){
        while(*last_target){
            if(abs((*last_target)->X, obj->X) <= range)
                if(abs((*last_target)->Y, obj->Y) <= range){
                    last_target++;
                    return *(last_target-1);
                }

            last_target++;
        }
    }

    last_target = NULL;
    return NULL;
}