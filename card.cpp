#include "card.hpp"

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
    }

    void za_mapo::update_card () {
        wprintw(win, (char*)mapa);

        int color;
        for(int i = 0; objects[i]; i++){
            switch(objects[i]->get_fraction()){
                case HUMANITY:
                    color = 2; break;
                case MONSTER:
                    color = 1; break;
                default:
                    color = 0; break;
            }
            wattron(win, COLOR_PAIR(color));
            wmove(win, objects[i]->Y, objects[i]->X);
            waddch(win, objects[i]->get_type());
            wattroff(win, COLOR_PAIR(color));
        }
        wrefresh(win);
    }

    void za_mapo::magnetic_search(object* from, object* to){
        if(from->Y < to->Y){ // down
            if(from->X == to->X && move(from, 'x')) return; // just down
            if(from->X < to->X){ // down and right
                if(move(from, 'c')) return;
                if(from->X - to->X > from->Y - to->Y)
                    if(move(from, 'd') || move(from, 'x'))
                        return;
                else
                    if(move(from, 'x') || move(from, 'd'));
            }
            else if(from->X > to->X){ // down and left
                if(move(from, 'z')) return;
                if(to->X - from->X > from->Y - to->Y)
                    if(move(from, 'a') || move(from, 'x'))
                        return;
                else
                    if(move(from, 'x') || move(from, 'a'));
            }
        }
        else if(from->Y > to->Y){ // up
            if(from->X == to->X && move(from, 'w')) return; // just up
            if(from->X < to->X){ // up and right
                if(move(from, 'e')) return;
                if(from->X - to->X > to->Y - from->Y)
                    if(move(from, 'd') || move(from, 'w'))
                        return;
                else
                    if(move(from, 'w') || move(from, 'd'));
            }
            else if(from->X > to->X){ // up and left
                if(move(from, 'q')) return;
                if(to->X - from->X > to->Y - from->Y)
                    if(move(from, 'a') || move(from, 'w'))
                        return;
                else
                    if(move(from, 'w') || move(from, 'a'));
            }
        }
        else move(from, from->X < to->X ? 'd' : 'a');
    }

    char za_mapo::move(object* obj, char direction){
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

    za_mapo::~za_mapo(){ 
        delwin(win);
        delete[] mapa;
    }
