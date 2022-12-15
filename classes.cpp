#include "classes.hpp"

humanoid::humanoid(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){
    equipment.push_back({{ARMORY_HEAD, NOTHING_ITEM}, NULL, state()});
    equipment.push_back({{ARMORY_HAND, NOTHING_ITEM}, NULL, state()});
    equipment.push_back({{ARMORY_FOOT, NOTHING_ITEM}, NULL, state()});
    equipment.push_back({{CLOTH_BODY, NOTHING_ITEM}, NULL, state()});
    equipment.push_back({{CLOTH_FOOT, NOTHING_ITEM}, NULL, state()});
    equipment.push_back({{AMULET, NOTHING_ITEM}, NULL, state()});
    equipment.push_back({{HOLDABLE_ONE_HAND, NOTHING_ITEM}, NULL, state()});
    equipment.push_back({{HOLDABLE_ONE_HAND, NOTHING_ITEM}, NULL, state()});
}

target::target(int8_t _X, int8_t _Y) : object(_X, _Y, string("")) {}

action_t target::turn(){ return ACT_STAND; }


mage::mage(int8_t _X, int8_t _Y, string _name) : humanoid(_X, _Y, _name){
    stat.propertyes.insert({HEALTH, 20});
    stat.propertyes.insert({MANA, 50});
    stat.propertyes.insert({STRENGTH, 10});

    stat.effects.insert({{1,0,MANA_RESTORE}, {0, 0, 2}});

    spells.insert({FIREBALL, {1}});
    spells.insert({PUNCH, {1}});
    spells.insert({LIGHTING, {1}});

    fract = HUMANITY;
}

action_t mage::turn(){
    switch(behavior){
        case BHV_CHILL:
            return ACT_WALK;

        case BHV_WORRIED:
            return ACT_SEARCH_ENEMY;

        case BHV_ATTACK:
            return ACT_ATTACK;

        case BHV_PANIC:
            return ACT_RUN_AVAY;

        case BHV_STUNNED:
            return ACT_STAND;

        case BHV_REST:
        default:
            return ACT_REST;
    }
}


warrior::warrior(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){}

// todo just test. Not release
action_t warrior::turn(){ return ACT_ATTACK; }


goblin::goblin(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){}

// todo just test. Not release
action_t goblin::turn(){ return ACT_RUN_AVAY; }


chair::chair(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){}

action_t chair::turn(){ return ACT_STAND; }


golem::golem(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){
    stat.propertyes.insert({HEALTH, 50});
    stat.propertyes.insert({STRENGTH, 40});

    stat.effects.insert({{1,0,PHYSIC_PROTECT}, {0, 0, 30}});
    stat.effects.insert({{1,0,ELECTRIC_WEAKNESS}, {0, 0, 10}});
    stat.effects.insert({{1,0,UNFLAMED}, {0, 0, 0}});
    
    spells.insert({PUNCH, {1}});

    inventory.push_back({{USEABLE_ITEM, STONE}, NULL, state()});
    inventory.push_back({{USEABLE_ITEM, STONE}, NULL, state()});
    inventory.push_back({{USEABLE_ITEM, STONE}, NULL, state()});

    fract = MONSTER;
}

action_t golem::turn(){
    switch(behavior){
        case BHV_CHILL:
            return ACT_STAND;
        
        case BHV_WORRIED:
            return ACT_SEARCH_ENEMY;

        default:
            return ACT_ATTACK;
    }
}
