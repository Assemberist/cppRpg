#pragma once

class log{
public:
    virtual void newline(const char* src) = 0;
};

template <class C> class logger_of_shit{
    inline static log* spamer = 0;
public:
    static void set_log(log* src){ spamer = src; }
    static void newline(const char* src){ if(spamer) spamer->newline(src); }
};