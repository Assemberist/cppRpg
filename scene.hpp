#pragma once

#include "visual/curses/battle_scene.hpp"
#include <stdlib.h>

void init_graphic();
bool game_loop(object** objs, object* gamer, screen& s);