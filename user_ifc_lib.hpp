#pragma once

#include "card.hpp"
#include "classes.hpp"
#include "actions.hpp"

typedef std::pair<const spell_t, spell> spell_menu_content;

class spell_menu : public menu<spell_menu_content*>{
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

    bool is_current_equiped();
    // void invert_equip();
};


union inventory_content_2{
    object* owner;
    inventory_content content;

    inventory_content_2(){ owner = nullptr; }
};

class inventory_with_owner : public menu<inventory_content_2>{
    bool isActive;

public:
    inventory_with_owner(size_t rows, size_t cols, size_t pos_y, size_t pos_x);
    void build_content(object* obj);
    void print();
    bool is_current_equiped();
};

void print_help_for_stay(text_log* help);
void print_help_for_spell_choose(text_log* help);
void print_help_for_spell_target_choose_single(text_log* help);
void print_help_for_spell_target_choose_ranged(text_log* help);
void print_help_for_observation_card(text_log* help);
void print_help_for_observation_objects(text_log* help);
void print_help_for_observation_state(text_log* help);
void print_help_for_loot(text_log* help);
void print_help_for_inventory(text_log* help);
void print_help_for_loot_inventory(text_log* help);
void print_help_for_item_using(text_log* help);
