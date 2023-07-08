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
    GREEN_NO_INVERT,

    GRAY_STABILE
};

class drawable_object{
public:
    uint8_t X;
    uint8_t Y;

    blink_t graph_state;

    drawable_object(uint8_t _X, uint8_t _Y) : X(_X), Y(_Y) {}
    virtual char get_type() = 0;
};