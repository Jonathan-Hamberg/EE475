#ifndef MY_BUTTON_LISTENER_H
#define MY_BUTTON_LISTENER_H

#include <Arduino.h>
#include <Button.h>
#include <RGB_LED.h>
#include <ShiftOut.h>
#include <LiquidCrystal.h>
#include "Seg7.h"


class MyButtonListener : public ButtonListener {
  public:

  MyButtonListener(RGB_LED *led, Seg7 *seg, ShiftOut *out, LiquidCrystal *lcd, unsigned int n_up, unsigned int n_down);

  virtual void onEvent(Button* caller, ButtonEvent event);


  private:
  RGB_LED *led;
  Seg7 *seg;
  LiquidCrystal *lcd;
  ShiftOut *out;
  unsigned int n_up, n_down, value;
};

#endif // MY_BUTTON_LISTENER_H
