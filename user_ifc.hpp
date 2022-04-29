#pragma once

#include "card.hpp"
#include "classes.hpp"
#include "object_defs.hpp"

// to do 1: object target among all objects

enum object_state{
    STAY,
    CHOOSE_SPELL,
    CHOOSE_TARGET
};

bool user_turn(blink_cfg* u, screen s);
