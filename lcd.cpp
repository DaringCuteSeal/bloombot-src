
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "cfg.h"
#include "sensors.h"

enum LCDContextState
{
    HOME,
    STATS
};

enum Status
{
    HEALTHY,
    NEEDS_SUNLIGHT,
    TOO_HOT,
    TOO_COLD,
    TOO_HUMID, // air humidity
    TOO_DRY,
    SOIL_TOO_HUMID,
    SOIL_TOO_DRY
};

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

// Renderers handle the drawing of a screen

// Base renderer class
class LCDScreenRenderer
{
public:
    virtual void render_screen() = 0;
    virtual void update_state() = 0;
};

String multiply_string(byte amount, char character)
{
    String result = "";
    for (byte i = 0; i < amount; i++)
    {
        result += character;
    }
    return result;
}

Status get_plant_status(float temperature, float humidity, float soil_moisture)
{
    // Does not handle multiple statuses at once
    if (temperature < IDEAL_TEMP - 5)
    {
        return Status::TOO_COLD;
    }
    else if (temperature > IDEAL_TEMP + 7)
    {
        return Status::TOO_HOT;
    }
    else if (humidity < IDEAL_HUMIDITY - 5)
    {
        return Status::TOO_DRY;
    }
    else if (humidity > IDEAL_HUMIDITY + 5)
    {
        return Status::TOO_HUMID;
    }
    else if (soil_moisture < IDEAL_SOIL_MOISTURE - 5)
    {
        return Status::SOIL_TOO_DRY;
    }
    else if (soil_moisture > IDEAL_SOIL_MOISTURE + 5)
    {
        return Status::SOIL_TOO_HUMID;
    }
    else if (temperature > IDEAL_TEMP - 5 && temperature < IDEAL_TEMP + 7 && humidity > IDEAL_HUMIDITY - 5 && humidity < IDEAL_HUMIDITY + 5 && soil_moisture > IDEAL_SOIL_MOISTURE - 5 && soil_moisture < IDEAL_SOIL_MOISTURE + 5)
    {
        return Status::HEALTHY;
    }
}

String get_string_from_status(Status status)
{
    switch (status)
    {
    case Status::HEALTHY:
        return "OK";
    case Status::NEEDS_SUNLIGHT:
        return "GLP";
    case Status::TOO_HOT:
        return "PNS";
    case Status::TOO_COLD:
        return "DGN";
    case Status::TOO_HUMID:
        return "LBP";
    case Status::TOO_DRY:
        return "U_KRG";
    case Status::SOIL_TOO_HUMID:
        return "BSH";
    case Status::SOIL_TOO_DRY:
        return "T_KRG";
    default:
        return "Unknown";
    }
}

// Renders home screen. Also handles power-saving feature by turning off the screen when not in use
class HomeRenderer : public LCDScreenRenderer
{
public:
    HomeRenderer()
    {
        this->_lcd_status = true;
        this->_cached_temperature = 0.0;
        this->_cached_humidity = 0.0;
        this->_cached_soil_moisture = 0.0;
    }

    void render_screen()
    {
        // Render the home screen
        lcd.setCursor(0, 0);
        lcd.print(PROTO_PLANT_TYPE);
        lcd.print(": ");
        String plant_status = get_string_from_status(get_plant_status(this->_cached_temperature, this->_cached_humidity, this->_cached_soil_moisture));
        lcd.setCursor(LCD_COLS - plant_status.length(), 0);
        lcd.print(plant_status);

        // soil moisture
        lcd.setCursor(0, 1);
        lcd.print("H:");
        lcd.print(this->_cached_soil_moisture);
        lcd.print("% ");

        // temperature
        lcd.print("T:");
        lcd.print(int(this->_cached_temperature));
        lcd.print("*C ");

        // light intensity
        lcd.print("C:");
        // lcd.print(int());
        lcd.print("40% ");
    }

    void update_state()
    {
        if (!this->_lcd_status)
            return; // only update if screen is on
        this->_cached_temperature = dht22.read_temperature();
        this->_cached_humidity = dht22.read_humidity();
        this->_cached_soil_moisture = hygrometer.read();
    }

    void turn_screen_on()
    {
        lcd.backlight();
        this->_lcd_status = true;
    }

    void turn_screen_off()
    {
        lcd.noBacklight();
        this->_lcd_status = false;
    }

private:
    bool _lcd_status;
    float _cached_temperature;
    float _cached_humidity;
    float _cached_soil_moisture;
};

class LCDScreenManager
{
public:
    LCDScreenManager(void (*renderer)())
    {
        this->renderer = renderer;
    }

    void set_renderer(void (*renderer)())
    {
        this->renderer = renderer;
    }

    void render()
    {
        this->renderer();
    }

private:
    void (*renderer)();
};
