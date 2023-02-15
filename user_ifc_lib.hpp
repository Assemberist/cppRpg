#pragma once

#include "card.hpp"
#include "classes.hpp"
#include "actions.hpp"

class spell_menu : public menu{
public:
    spell_menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x);
    void print();
};

class inventory : public menu{
protected:
    bool isActive;

public:
    inventory(size_t rows, size_t cols, size_t pos_y, size_t pos_x);
    void print();
    // bool is_current_equiped();
    // void invert_equip();
    void activate(size_t num);
    void deactivate();
};

class object_observe_menu : public menu{
public:
    object_observe_menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x);
    void print();
};


// *********************//
// Internal structures  //
// *********************//

/*

log* create_log_effects(state* stat);
log* create_log_properties(state* stat);

// Spell menu window

struct menu_element{
    int name;
    void* element;
};

void print_menu(WINDOW* win, size_t count, size_t current, void** elements, char** strings);

// Inventory windows 

struct bag_element{
    bool is_equiped :1;
    size_t type :31;
    size_t element;
};

void** create_bag(object* u, size_t& sas);
void** create_bag_with_owner(object* u, size_t& sas);

void print_inventory(WINDOW* win, size_t count, size_t current, void** elements, char** strings);
void print_inventory_active(WINDOW* win, size_t count, size_t current, void** elements, char** strings);

*/