#pragma once

#include "card.hpp"
#include "classes.hpp"
#include "actions.hpp"

typedef std::pair<const spell_t, spell> spell_menu_content;

class spell_menu : menu<spell_menu_content*>{
public:
    spell_menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x);
    void build_content(object* obj);
    void print();
};


struct inventory_content{
    bool is_equiped :1;
    vector<item>::iterator it;
};

class inventory : public menu<inventory_content>{
    bool isActive;

public:
    inventory(size_t rows, size_t cols, size_t pos_y, size_t pos_x);
    void build_content(object* obj);
    void print();

    void activate(size_t num);
    void deactivate();

    // bool is_current_equiped();
    // void invert_equip();
};


union inventory_content_2{
    object* owner;
    inventory_content content;
};

class inventory_with_owner : public menu<inventory_content_2>{
    bool isActive;

public:
    inventory_with_owner(size_t rows, size_t cols, size_t pos_y, size_t pos_x);
    void build_content(object* obj);
    void print();
};