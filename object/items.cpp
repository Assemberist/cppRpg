#include "items.hpp" 

const char* item_names[] = {
    "Wood Shield\n",
    "Wood Sword\n",
    "Wood Bow\n",
    
    "IRON SHIELD\n",
    "IRON SWORD\n",
    "IRON HUMMER\n",
    "IRON BOW\n",

    "JACKET\n",
    "JEANS\n",

    "LATE HELM\n",
    "LATE ARMORY\n",
    "LATE GLOVE\n",
    "LATE FOOT\n",

    "Latex helm\n",
    "Latex armory\n",
    "Latex Glove\n",
    "Latex foot\n",

    "Ice Crystall\n",
    "Acid colba\n",
    "Stone\n",
    "Healing rune\n",
    "Sintol\n",

    "NOTHING\n"
};

bool is_item_for_single_target(item_t thing){
    switch(thing){
        case ICE_CRYSTALL:
        case ACID_COLBA:
            return false;

        default:
            return true;
    }
}
