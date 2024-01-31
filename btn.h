#ifndef BTN_H
#define BTN_H

#include <Button.h>
#include <ButtonEventCallback.h>
#include <BasicButton.h>

extern BasicButton btn_nav_menu;
extern BasicButton btn_nav_left;
extern BasicButton btn_nav_righ;

void init_btns();

void update_btns();

#endif