#include <Arduino.h>
#include <SoilHygrometer.h>
#include <DHT.h>
#include <DHT_U.h>
#include "pins.h"

SoilHygrometer lib_hygrometer(PIN_ANALOG_SENSOR_SOIL_MOISTURE);
DHT lib_dht22(DHTPIN, DHTTYPE);

class Sensor_SoilHygrometer
{
public:
    Sensor_SoilHygrometer()
    {
        this->_value = 0.0;
        this->_active = true;
    }

    void periodic_timer_update_value()
    {
        if (!this->_active)
            return;
        this->_value = lib_hygrometer.read(0, 100);
    }

    float read()
    {
        return this->_value;
    }

    void set_active_state(bool active)
    {
        this->_active = active;
    }

private:
    float _value;
    bool _active;
};

class Sensor_DHT22
{
public:
    Sensor_DHT22()
    {
        this->_temperature = 0.0;
        this->_humidity = 0.0;
        this->_active = true;
        this->begin();
    }

    void begin()
    {
        lib_dht22.begin();
    }

    void periodic_timer_update_value()
    {
        if (!this->_active)
            return;
        this->_temperature = lib_dht22.readTemperature();
        this->_humidity = lib_dht22.readHumidity();
    }

    float read_temperature()
    {
        return this->_temperature;
    }

    float read_humidity()
    {
        return this->_humidity;
    }

    void set_active_state(bool active)
    {
        this->_active = active;
    }

private:
    float _temperature;
    float _humidity;
    bool _active;
};

class StatisticCounter
{
public:
    StatisticCounter()
    {
        this->_auto_watering_count = 0;
        this->_reminders_count = 0;
    }

    void increment_auto_watering_count()
    {
        this->_auto_watering_count++;
    }
    void increment_reminders_count()
    {
        this->_reminders_count++;
    }
    int get_watering_count()
    {
        return this->_auto_watering_count;
    }
    int get_reminders_count()
    {
        return this->_reminders_count;
    }

    int get_uptime()
    {
        return this->_uptime; // in second
    }
    void update_uptime()
    {
        this->_uptime += 1; // updates every second
    }

private:
    int _auto_watering_count;
    int _reminders_count;
    unsigned long _uptime;
};
;

Sensor_SoilHygrometer hygrometer;
Sensor_DHT22 dht22;
StatisticCounter statistic_counter;

void hygrometer_periodic_update_value()
{
    hygrometer.periodic_timer_update_value();
};

void dht22_periodic_update_value()
{
    dht22.periodic_timer_update_value();
};

void statistic_counter_periodic_update_value()
{
    statistic_counter.update_uptime();
};

float get_humidity()
{
    return dht22.read_humidity();
}

float get_temperature()
{
    return dht22.read_temperature();
}

float get_soil_moisture()
{
    return 100 - hygrometer.read();
}

int get_watering_count()
{
    return statistic_counter.get_watering_count();
}
int get_uptime()
{
    return statistic_counter.get_uptime();
}
