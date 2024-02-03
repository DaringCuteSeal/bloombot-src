// Kode Prototype PJBL Future Founders
// Written by DaringCuteSeal (https://github.com/DaringCuteSeal/BloomBot)

// Libraries

#include <Ticker.h>
#include <Button.h>
#include <ButtonEventCallback.h>
#include <BasicButton.h>

#include "pins.h"
#include "pump.h"
#include "sensors.h"
#include "lcd.h"
#include "timekeeping.h"

// Timers
void turn_screen_off();
void main_poll_pump_timer_fn()
{
    Serial.print(poll_pump());
    if (poll_pump())
    {
        start_pump();
    }
    else
    {
        stop_pump();
    };
}

Ticker hygrometer_timer(hygrometer_periodic_update_value, 1000, 0, MILLIS);
Ticker dht22_timer(dht22_periodic_update_value, 1000, 0, MILLIS);
Ticker statistic_counter_timer(statistic_counter_periodic_update_value, 1000, 0, MILLIS);
Ticker increment_uptime_timer(increment_uptime, 1000, 0, MILLIS);
Ticker update_lcd_timer(lcd_update, 1000, 0, MILLIS);
Ticker render_lcd_timer(lcd_render, 5000, 0, MILLIS);
Ticker turn_screen_off_timer(turn_screen_off, 10000, 0, MILLIS);
Ticker main_poll_pump_timer(main_poll_pump_timer_fn, 2000, 0, MILLIS);

void turn_screen_off()
{
    turn_screen_off_timer.stop();
    // turn_off_lcd();
}

void delayed_turn_screen_off(Button &btn)
{
    if (get_context() == LCDContextState::STATS)
        return;
    lcd_btn_menu_handler(btn);
    turn_screen_off_timer.start();
}

// Buttons
BasicButton btn_menu(PIN_DIGITAL_NAV_MENU);
BasicButton btn_left(PIN_DIGITAL_NAV_LEFT);
BasicButton btn_right(PIN_DIGITAL_NAV_RIGHT);

void init_btns()
{
    btn_menu.onHold(3000, lcd_btn_menu_hold_handler);
    btn_menu.onPress(delayed_turn_screen_off);
    btn_left.onPress(lcd_btn_left_handler);
    btn_right.onPress(lcd_btn_right_handler);
}

void update_btns()
{
    btn_menu.update();
    btn_left.update();
    btn_right.update();
}

void start_ticker_timers()
{
    hygrometer_timer.start();
    dht22_timer.start();
    statistic_counter_timer.start();
    increment_uptime_timer.start();
    update_lcd_timer.start();
    render_lcd_timer.start();
    main_poll_pump_timer.start();
}
void update_ticker_timers()
{
    hygrometer_timer.update();
    dht22_timer.update();
    statistic_counter_timer.update();
    increment_uptime_timer.update();
    update_lcd_timer.update();
    render_lcd_timer.update();
    turn_screen_off_timer.update();
    main_poll_pump_timer.update();
}

void setup()
{
    Serial.begin(9600);
    start_ticker_timers();
    init_btns();
    init_lcd();
    delayed_turn_screen_off(btn_menu);
    digitalWrite(PIN_DIGITAL_WATER_PUMP_CTRL, LOW);
}

void loop()
{
    update_ticker_timers();
    update_btns();
}
