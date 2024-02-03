#ifndef SENSORS_H
#define SENSORS_H
#include <Arduino.h>
#include <SoilHygrometer.h>
#include <DHT.h>
#include <DHT_U.h>
#include "pins.h"

class Sensor_SoilHygrometer
{
public:
    Sensor_SoilHygrometer();
    void periodic_timer_update_value();
    float read();
    void set_active_state(bool active);

private:
    float _value;
    bool _active;
};

class Sensor_DHT22
{
public:
    Sensor_DHT22();
    void begin();
    void periodic_timer_update_value();
    float read_temperature();
    float read_humidity();
    void set_active_state(bool active);

private:
    float _temperature;
    float _humidity;
    bool _active;
};

class StatisticCounter
{
public:
    StatisticCounter();
    void increment_auto_watering_count();
    void increment_reminders_count();
    int get_watering_count();
    int get_reminders_count();
    int get_uptime();
    void update_uptime();

private:
    int _auto_watering_count;
    int _reminders_count;
    unsigned long _uptime;
};

void hygrometer_periodic_update_value();

void dht22_periodic_update_value();

void statistic_counter_periodic_update_value();
float get_humidity();

float get_temperature();

float get_soil_moisture();

int get_watering_count();
int get_uptime();
#endif
