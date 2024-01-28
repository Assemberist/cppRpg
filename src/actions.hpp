#pragma once

#include "object/object.hpp"

void do_action(object* obj, object* target, spell_t sp);

void do_nothing(object*, object*);
void act_punch(object* obj, object* target);
void act_lighting(object* obj, object* target);
void act_throw(object* obj, object* target);
// todo
void act_fireball(object* obj, object* target);