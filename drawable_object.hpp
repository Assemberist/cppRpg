#pragma once

#include <cstdint>

enum blink_t{
    HIDE,

    RED_STABILE,
    RED_ON,
    RED_OFF,
    RED_INVERT,
    RED_NO_INVERT,

    GREEN_STABILE,
    GREEN_ON,
    GREEN_OFF,
    GREEN_INVERT,
    GREEN_NO_INVERT
};

class drawable_object{
public:
    uint8_t X;
    uint8_t Y;

    blink_t graph_state;

    drawable_object(uint8_t _X, uint8_t _Y) : X(_X), Y(_Y) {}
    virtual char get_type() = 0;

    // card.cpp uses this but it is violation.
    // it should be removed after it will be fixed.
    virtual bool is_alive() = 0;
};