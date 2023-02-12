#pragma once

#include "card.hpp"
#include "classes.hpp"
#include "actions.hpp"

// to do 1: object target among all objects

bool user_turn(object* u, screen s);

// *********************//
// Internal structures  //
// *********************//

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
