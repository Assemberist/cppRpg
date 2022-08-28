#pragma once
#include "object.hpp"

class humanoid : public object{
public:
    humanoid(int8_t _X, int8_t _Y, string _name);
};

class target : public object{
public:
    target(int8_t _X, int8_t _Y);
    char get_type(){ return 'x'; }
    action_t turn();
};

class mage : public humanoid{
public:
    mage(int8_t _X, int8_t _Y, string _name);
    char get_type(){ return 'm'; }
    action_t turn();
};

class warrior : public object{
public:
    warrior(int8_t _X, int8_t _Y, string _name);
    char get_type(){ return 'w'; }
    action_t turn();
};

class goblin : public object{
public:
    goblin(int8_t _X, int8_t _Y, string _name);
    char get_type(){ return 'g'; }
    action_t turn();
};

class chair : public object{
public:
    chair(int8_t _X, int8_t _Y, string _name);
    char get_type(){ return 'h'; }
    action_t turn();
};

class golem : public object{
public:
    golem(int8_t _X, int8_t _Y, string _name);
    char get_type(){ return 'G'; }
    action_t turn();
};
