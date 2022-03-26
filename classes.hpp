#pragma once
#include "object.hpp"

class mage : public object{
public:
    mage(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){}
    char get_type();
};

char mage::get_type(){ return 'm'; }

class warrior : public object{
public:
    char get_type();
};

char warrior::get_type(){ return 'w'; }

class goblin : public object{
public:
    goblin(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){}
    char get_type();
};

char goblin::get_type(){ return 'g'; }

class chair : public object{
public:
    char get_type();
};

char chair::get_type(){ return 'h'; }
