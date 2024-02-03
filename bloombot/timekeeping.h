#ifndef TIMEKEEPING_H
#define TIMEKEEPING_H

#include <Arduino.h>

class SystemTimeKeeper
{
public:
    SystemTimeKeeper();
    unsigned long int get_uptime() const;

private:
    unsigned long _full_uptime;
    unsigned long _counter_uptime;
};

void increment_uptime();
#endif