
#ifndef SENSORS_H
#define SENSORS_H
#include <SoilHygrometer.h>
#include <DHT.h>
#include <DHT_U.h>
#include "pins.h"

class Sensor_SoilHygrometer
{
public:
    Sensor_SoilHygrometer() : _lib_hygrometer(PIN_ANALOG_SENSOR_SOIL_MOISTURE)
    {
        this->_value = 0.0;
        this->_active = true;
    }

    void periodic_timer_update_value()
    {
        if (!this->_active)
            return;
        this->_value = this->_lib_hygrometer.read(0, 100);
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
    SoilHygrometer _lib_hygrometer;
    float _value;
    bool _active;
};

class Sensor_DHT22
{
public:
    Sensor_DHT22() : _lib_dht22(DHTPIN, DHTTYPE)
    {
        this->_temperature = 0.0;
        this->_humidity = 0.0;
    }

    void begin()
    {
        this->_lib_dht22.begin();
    }

    void periodic_timer_update_value()
    {
        if (!this->_active)
            return;
        this->_temperature = this->_lib_dht22.readTemperature();
        this->_humidity = this->_lib_dht22.readHumidity();
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
    DHT _lib_dht22;
    float _temperature;
    float _humidity;
    bool _active;
};

extern Sensor_SoilHygrometer hygrometer;
extern Sensor_DHT22 dht22;
#endif