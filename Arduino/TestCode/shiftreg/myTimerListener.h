#ifndef MY_TIMER_LISTENER_H
#define MY_TIMER_LISTENER_H

#include "Timer.h"
#include <Arduino.h>
#include <RGB_LED.h>
#include <LiquidCrystal.h>
#include "LFSR.h"


class MyTimerListener : public TimerListener {
  public:

    MyTimerListener(RGB_LED* led, lfsr* r, LiquidCrystal *lcd, unsigned int n);
    
    virtual unsigned int onEvent(Timer *caller, unsigned int id, unsigned int calls);
  private:
    RGB_LED *led;
    LiquidCrystal *lcd;
    lfsr *r;
    unsigned int n;
  
};

#endif // MY_TIMER_LISTENER_H
