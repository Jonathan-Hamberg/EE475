#include <Arduino.h>
#include "myTimerListener.h"


MyTimerListener::MyTimerListener(RGB_LED *led, lfsr* r, LiquidCrystal *lcd, unsigned int n) {
  this->led = led;
  this->n = n;
  this->r = r;
  this->lcd = lcd;
}

unsigned int MyTimerListener::onEvent(Timer *caller, unsigned int id, unsigned int calls) {
  lcd->setCursor(0, 0);
  lcd->print("LEDS  :");
  lcd->setCursor(8 + id, 0);
  (void)(caller); // Unused parameter warning.

  int color = calls & 7;

  switch (color) {
    case LED_BLACK :
      lcd->print(" ");
      break;
    case LED_BLUE :
      lcd->print("B");
      break;
    case LED_GREEN :
      lcd->print("G");
      break;
    case LED_CYAN :
      lcd->print("C");
      break;
    case LED_RED :
      lcd->print("R");
      break;
    case LED_YELLOW :
      lcd->print("Y");
      break;
    case LED_PURPLE :
      lcd->print("M");
      break;
    default :
      lcd->print("W");
  }
  
  led->set(id, calls);
  return 300;
}

