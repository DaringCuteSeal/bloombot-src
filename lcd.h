#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

// Base renderer class
class LCDScreenRenderer
{
public:
    virtual void render_screen() = 0;
    virtual void update_state() = 0;
};

String multiply_string(byte amount, char character);

enum class Status
{
    HEALTHY,
    NEEDS_SUNLIGHT,
    TOO_HOT,
    TOO_COLD,
    TOO_HUMID,
    TOO_DRY,
    SOIL_TOO_HUMID,
    SOIL_TOO_DRY
};

Status get_plant_status(float temperature, float humidity, float soil_moisture);

String get_string_from_status(Status status);

// Renders home screen. Also handles power-saving feature by turning off the screen when not in use
class HomeRenderer : public LCDScreenRenderer
{
public:
    HomeRenderer();

    void render_screen() override;

    void update_state() override;

    void turn_screen_on();

    void turn_screen_off();

private:
    bool _lcd_status;
    float _cached_temperature;
    float _cached_humidity;
    float _cached_soil_moisture;
};

class LCDScreenManager
{
public:
    LCDScreenManager(void (*renderer)());

    void set_renderer(void (*renderer)());
    void render();

private:
    LCDScreenRenderer *renderer;
};

#endif // LCD_H
