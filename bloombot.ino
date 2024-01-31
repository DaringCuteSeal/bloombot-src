// Kode Prototype PJBL Future Founders
// Written by DaringCuteSeal (https://github.com/DaringCuteSeal/BloomBot)

// Libraries

#include <Ticker.h>
#include "pins.h"
#include "sensors.h"
#include "btn.h"
#include "lcd.h"

// Timers
Ticker hygrometer_timer([]()
                        { hygrometer.periodic_timer_update_value(); },
                        1000, 0, MILLIS);

Ticker dht22_timer([]()
                   { dht22.periodic_timer_update_value(); },
                   1000, 0, MILLIS);

// Buttons

HomeRenderer home_renderer;
LCDScreenManager lcd_menu(&home_renderer.render_screen);

void setup()
{
    Serial.begin(9600);
    // init_btns();
}

void loop()
{
    update_btns();
    lcd_menu.render();
    lcd_menu.update();
}