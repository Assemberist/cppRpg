#include "items.hpp" 

bool is_item_for_single_target(item_t thing){
    switch(thing){
        case ICE_CRYSTALL:
        case ACID_COLBA:
            return false;

        default:
            return true;
    }
}
