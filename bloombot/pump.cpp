#include <Arduino.h>
#include "sensors.h"
#include "cfg.h"

class PumpController
{
public:
    bool poll()
    {
        if (get_soil_moisture() < IDEAL_SOIL_MOISTURE)
        {
            return true;
        }
        return false;
    }

    void water()
    {
        float soil_moisture;
        while (soil_moisture = get_soil_moisture())
        {
            if (soil_moisture > IDEAL_SOIL_MOISTURE)
                break;
        }
    }
    void start_pump()
    {
        digitalWrite(PIN_DIGITAL_WATER_PUMP_CTRL, HIGH);
    }

    void stop_pump()
    {
        digitalWrite(PIN_DIGITAL_WATER_PUMP_CTRL, LOW);
    }
};

PumpController pump_controller;
void start_pump()
{
    pump_controller.start_pump();
}

void stop_pump()
{
    pump_controller.stop_pump();
}

bool poll_pump()
{
    Serial.print(pump_controller.poll());
    return pump_controller.poll();
}