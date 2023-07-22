#pragma once
#include "object.hpp"

enum object_t{
    SERVICE,
    HUMAN,
    GOBLIN,
    ORK,
    GOLEM,
    DOG,
    ELF,
    LIZZARD,
    DEMON,
    SUCCUBUS,
    SKELETON,
    ZOMBIE
};

enum class_t{
    MAGE,
    WARRIOR,
    RANGER
};

class humanoid : public object{
public:
    humanoid(int8_t _X, int8_t _Y, string _name);
    size_t get_obj_type(){ return HUMAN; }
};

class target : public object{
public:
    target(int8_t _X, int8_t _Y);
    char get_type(){ return 'x'; }
    size_t get_obj_type(){ return SERVICE; }
    action_t turn();
};

class mage : public humanoid{
public:
    mage(int8_t _X, int8_t _Y, string _name);
    char get_type(){ return 'm'; }
    action_t turn();
};

class warrior : public humanoid{
public:
    warrior(int8_t _X, int8_t _Y, string _name);
    char get_type(){ return 'w'; }
    action_t turn();
};

class goblin : public object{
public:
    goblin(int8_t _X, int8_t _Y, string _name);
    char get_type(){ return 'g'; }
    size_t get_obj_type(){ return GOBLIN; }
    action_t turn();
};

class chair : public object{
public:
    chair(int8_t _X, int8_t _Y, string _name);
    char get_type(){ return 'h'; }
    size_t get_obj_type(){ return SERVICE; }
    action_t turn();
};

class golem : public object{
public:
    golem(int8_t _X, int8_t _Y, string _name);
    char get_type(){ return 'G'; }
    size_t get_obj_type(){ return GOLEM; }
    action_t turn();
};
