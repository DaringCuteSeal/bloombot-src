#include <Button.h>
#include <Arduino.h>
#include <ButtonEventCallback.h>
#include <BasicButton.h>
#include "pins.h"
#include "lcd.h"

BasicButton btn_nav_menu = BasicButton(PIN_DIGITAL_NAV_MENU);
BasicButton btn_nav_left = BasicButton(PIN_DIGITAL_NAV_LEFT);
BasicButton btn_nav_right = BasicButton(PIN_DIGITAL_NAV_RIGHT);

void update_btns()
{
    btn_nav_menu.update();
    btn_nav_left.update();
    btn_nav_right.update();
}

void fire_btn_event_menu(Button &btn)
{
    // The button was pressed - do something!
    Serial.print("AA");
}

void fire_btn_event_left(Button &btn)
{
    Serial.print("AA");

    // The button was pressed - do something!
}

void fire_btn_event_ok(Button &btn)
{
    Serial.print("AA");

    // The button was pressed - do something!
}

void fire_btn_event_right(Button &btn)
{
    Serial.print("AA");

    // The button was pressed - do something!
}

void init_btns()
{
    btn_nav_menu.onPress(fire_btn_event_menu);
    btn_nav_left.onPress(fire_btn_event_left);
    btn_nav_right.onPress(fire_btn_event_right);
}