#include <Arduino.h>

class SystemTimeKeeper
{
public:
    SystemTimeKeeper()
    {
        this->_uptime = 0;
    }

    unsigned long int get_uptime() const
    {
        return this->_uptime;
    }
    void increment()
    {
        this->_uptime += 1;
    }

private:
    unsigned long int _uptime;
};

SystemTimeKeeper timekeeper;

void increment_uptime()
{
    timekeeper.increment();
}