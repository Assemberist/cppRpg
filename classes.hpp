#pragma once
#include "object.hpp"

class target : public object{
public:
    target(int8_t _X, int8_t _Y) : object(_X, _Y, string("")) {}
    void print_spells(menu* _menu) {}
    char get_type(){ return 'x'; }
};

class mage : public object{
public:
    mage(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){
        propertyes.insert({HEALTH, 20});
        propertyes.insert({MANA, 50});

        effects.insert({MANA_RESTORE, {0, 2}});

        fract = HUMANITY;
    }

    void print_spells(menu* _menu){
        _menu->clear();
        _menu->putline("Fire ball\n");
        _menu->putline("Lighting\n");
        _menu->putline("Punch\n");
        _menu->select(0);
        _menu->print();
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

        effects.insert({PHYSIC_PROTECT, {0, 30}});
        effects.insert({ELECTRIC_WEAKNESS, {0, 10}});
        effects.insert({UNFLAMED, {0, 0}});
        
        fract = MONSTER;
    }

    void print_spells(menu* _menu){
        _menu->clear();
        _menu->putline("Punch\n");
        _menu->select(0);
        _menu->print();
    }

    char get_type(){ return 'G'; }
};
