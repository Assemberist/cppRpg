#pragma once

#include <string>
#include "object.hpp"

using namespace std;

char move(char (*arr)[12], object* obj, char direction);

void magnetic_search(char (*map)[12], object* from, object* to);
