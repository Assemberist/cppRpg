#pragma once

#pragma once

#include "text_field.hpp"

class log : public text_field{
public:
    void print();
    void newline(const char* src);
};