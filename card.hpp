#pragma once

#include "object.hpp"
#include <ncurses.h>
#include <stdint.h>

void drow_card(char (*card)[12], object** objects, size_t obj_num);

void update_card(char (*card)[12], size_t rows);
