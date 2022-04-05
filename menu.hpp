#pragma once

#include "text_field.hpp"

class menu : public text_field{
public:
    void print();
    void newline(const char* src);

    void up();
    void down();
    void select(size_t num);
};