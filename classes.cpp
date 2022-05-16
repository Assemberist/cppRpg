#include "classes.hpp"

target::target(int8_t _X, int8_t _Y) : object(_X, _Y, string("")) {}

action_t target::turn(){ return ACT_STAND; }


mage::mage(int8_t _X, int8_t _Y, string _name) : object(_X, _Y, _name){
    propertyes.insert({HEALTH, 20});
    propertyes.insert({MANA, 50});

    effects.insert({MANA_RESTORE, {0, 2}});

    spells.push_back(_SPELL_FIREBALL);
    spells.push_back(_SPELL_PUNCH);
    spells.push_back(_SPELL_LIGTHING);

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
    propertyes.insert({HEALTH, 50});

    effects.insert({PHYSIC_PROTECT, {0, 30}});
    effects.insert({ELECTRIC_WEAKNESS, {0, 10}});
    effects.insert({UNFLAMED, {0, 0}});
    
    spells.push_back(_SPELL_PUNCH);

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
