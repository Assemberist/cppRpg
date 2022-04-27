#pragma once
#include "object.hpp"

class mage : public object{
public:
    mage(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){
        propertyes.insert({HEALTH, 20});
        propertyes.insert({MANA, 50});
        fract = HUMANITY;
    }

    void print_spells(menu* _menu){
        _menu.clear();
        _menu.newline("Fire ball");
        _menu.newline("Lighting");
        _menu.newline("Punch");
        _menu.select(0);
        _menu.print();
    }

    char get_type(){ return 'm'; }
};

class warrior : public object{
public:
    warrior(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){}
    char get_type(){ return 'w'; }
};

class goblin : public object{
public:
    goblin(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){}
    char get_type(){ return 'g'; }
};

class chair : public object{
public:
    chair(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){}
    char get_type(){ return 'h'; }
};

class golem : public object{
public:
    golem(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){
        propertyes.insert({HEALTH, 50});
        fract = MONSTER;
    }

    void print_spells(menu* _menu){
        _menu.clear();
        _menu.newline("Punch");
        _menu.select(0);
        _menu.print();
    }

    char get_type(){ return 'G'; }
};
