#pragma once
#include "object.hpp"

class mage : public object{
public:
    mage(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){
        propertyes.insert({HEALTH, 20});
        propertyes.insert({MANA, 50});
        fract = HUMANITY;
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
    char get_type(){ return 'G'; }
};
