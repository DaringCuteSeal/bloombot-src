
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "cfg.h"
#include "sensors.h"
#include <Ticker.h>
#include "timekeeping.h"
#include <Button.h>
#include <ButtonEventCallback.h>
#include <BasicButton.h>
#include "lcd.h"

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

// Renderers handle the drawing of a screen

// Base renderer class
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
};

void render_home_screen(float temperature, float humidity, float soil_moisture)
{
    lcd.clear();
    // Render the home screen
    lcd.setCursor(0, 0);
    lcd.print(PROTO_PLANT_TYPE);
    lcd.print(": ");
    String plant_status = get_string_from_status(get_plant_status(temperature, humidity, soil_moisture));
    lcd.setCursor(LCD_COLS - plant_status.length(), 0);
    lcd.print(plant_status);

    // soil moisture
    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(soil_moisture);
    lcd.print("% ");

    // temperature
    lcd.print("T:");
    lcd.print(int(temperature));
    lcd.print("*C ");

    // light intensity
    lcd.print("C:");
    // lcd.print(int());
    lcd.print("40% ");
};

String get_menu_item_label(byte idx)
{
    switch (idx)
    {
    case 0:
        return "Lmbp tnh";
    case 1:
        return "Suhu ruang";
    case 2:
        return "Lmbp udara";
    case 3:
        return "Penyiraman";
    case 4:
        return "Uptime";
    case 5:
        return "Aksi";
    default:
        return "Unknown";
    }
};

String get_menu_item_value(byte idx)
{
    switch (idx)
    {
    case 0:
        return String(get_soil_moisture());
    case 1:
        return String(get_temperature());
    case 2:
        return String(get_humidity());
    case 3:
        return String(get_watering_count());
    case 4:
        return String(get_uptime() + "s");
    case 5:
        return String("N/A");
    default:
        return "Unknown";
    }
};

void render_stats_menu(String label, String value, byte idx, byte max_idx)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stats");

    lcd.setCursor(LCD_COLS - String(max_idx).length() - String(idx).length() - 3, 0);
    lcd.print("[");
    lcd.print(idx);
    lcd.print("/");
    lcd.print(max_idx);
    lcd.print("]");

    lcd.setCursor(0, 1);
    if (idx > 0)
    {
        lcd.print("< ");
    };

    lcd.print(label);
    lcd.print(": ");
    lcd.print(value);

    if (idx == max_idx)
    {
        lcd.setCursor(LCD_COLS - 2, 1);
        lcd.print("> ");
    };
};

class LCDScreenManager
{
public:
    LCDScreenManager()
    {
        this->_context_state = LCDContextState::HOME;
        this->_menu_idx = 0;
        this->_max_menu_idx = 5;
        this->_lcd_status = true;
        this->_cached_temperature = 0;
        this->_cached_humidity = 0;
        this->_cached_soil_moisture = 0;
    }

    void update()
    {
        if (this->_lcd_status == false)
            return; // we only update when the screen is on
        this->_cached_humidity = get_humidity();
        this->_cached_temperature = get_temperature();
        this->_cached_soil_moisture = get_soil_moisture();
    }

    void render()
    {
        if (this->_lcd_status == false)
            return; // we only render when the screen is on
        if (this->_context_state == LCDContextState::HOME)
        {
            render_home_screen(this->_cached_temperature, this->_cached_humidity, this->_cached_soil_moisture);
        }
        else if (this->_context_state == LCDContextState::STATS)
        {
            render_stats_menu(get_menu_item_label(this->_menu_idx), get_menu_item_value(this->_menu_idx), this->_menu_idx, this->_max_menu_idx); // note: we don't use cached values because the LCD is always on when we're in the stats menu
        }
    }

    LCDContextState get_context()
    {
        return this->_context_state;
    }
    void btn_menu_long_press()
    {
        if (this->_lcd_status == false)
            return; // we only act on long press when the screen is on
        if (this->_context_state == LCDContextState::HOME)
        {
            this->_context_state = LCDContextState::STATS;
        }
        else if (this->_context_state == LCDContextState::STATS)
        {
            this->_context_state = LCDContextState::HOME;
        }
        this->render();
    }

    void btn_left()
    {
        if (this->_lcd_status == false)
            return; // we only act on button presses when the screen is on
        if (this->_context_state == LCDContextState::STATS)
        {
            if (this->_menu_idx > 0)
            {
                this->_menu_idx--;
            }
        }
        this->render();
    }

    void btn_right()
    {
        if (this->_lcd_status == false)
            return; // we only act on button presses when the screen is on
        if (this->_context_state == LCDContextState::STATS)
        {
            if (this->_menu_idx < this->_max_menu_idx)
            {
                this->_menu_idx++;
            }
        }
        this->render();
    }

    void turn_screen_off()
    {
        this->_lcd_status = false;
        lcd.noBacklight();
    }
    void btn_menu_short_press()
    {
        this->_lcd_status = true;
        lcd.backlight();
    }

private:
    float _cached_temperature;
    float _cached_humidity;
    float _cached_soil_moisture;
    bool _lcd_status;
    LCDContextState _context_state;
    byte _menu_idx;
    byte _max_menu_idx;
};

LCDScreenManager lcd_menu;

void lcd_render()
{
    lcd_menu.render();
}

void lcd_update()
{
    lcd_menu.update();
}

void lcd_btn_menu_handler(Button &btn)
{
    lcd_menu.btn_menu_short_press();
}

void lcd_btn_menu_hold_handler(Button &btn, uint16_t duration)
{

    lcd_menu.btn_menu_long_press();
}

void lcd_btn_left_handler(Button &btn)
{
    lcd_menu.btn_left();
}

void lcd_btn_right_handler(Button &btn)
{
    lcd_menu.btn_right();
}

void turn_off_lcd()
{
    lcd.noBacklight();
}

void init_lcd()
{
    lcd.begin(LCD_COLS, LCD_ROWS);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
}

LCDContextState get_context()
{
    lcd_menu.get_context();
}