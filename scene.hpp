#pragma once

#include "user_ifc.hpp"
#include <stdlib.h>

void init_graphic();
bool game_loop(object** objs, object* gamer, screen& s);