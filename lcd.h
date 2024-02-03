#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "cfg.h"
#include "sensors.h"
#include <Ticker.h>
#include "timekeeping.h"

enum LCDContextState
{
    HOME,
    STATS
};

extern LiquidCrystal_I2C lcd;

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

class LCDScreenRenderer
{
public:
    virtual void render_screen() = 0;
    virtual void update_state() = 0;
};

String multiply_string(byte amount, char character);
Status get_plant_status(float temperature, float humidity, float soil_moisture);
String get_string_from_status(Status status);
void render_home_screen(float temperature, float humidity, float soil_moisture);
String get_menu_item_label(byte idx);
String get_menu_item_value(byte idx);
void render_stats_menu(String label, String value, byte idx, byte max_idx);

void lcd_render();

void lcd_update();

void lcd_btn_menu_handler(Button &btn);

void lcd_btn_menu_hold_handler(Button &btn, uint16_t duration);

void lcd_btn_left_handler(Button &btn);

void lcd_btn_right_handler(Button &btn);

void init_lcd();

void turn_off_lcd();

LCDContextState get_context();
#endif