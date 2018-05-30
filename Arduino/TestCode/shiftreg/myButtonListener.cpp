#include <Arduino.h>
#include "myButtonListener.h"

MyButtonListener::MyButtonListener(RGB_LED *led, Seg7 *seg, ShiftOut *out, LiquidCrystal *lcd, unsigned int n_up, unsigned int n_down) {
  this->led = led;
  this->n_up = n_up;
  this->n_down = n_down;
  this->seg = seg;
  this->lcd = lcd;
  this->out = out;
  value = 0;
}

void MyButtonListener::onEvent(Button* caller, ButtonEvent event) {
  unsigned int id = caller->getID();
  lcd->setCursor(0,1);
  lcd->print("Button:");
  lcd->setCursor(8 + id, 1);

  if (id == 4) {
    if (ButtonEvent::PRESS == event) {
      out->set(38, 0, true);
    } else {
      out->set(38, 0, false);
    }
  }

  if (event == ButtonEvent::RELEASE) {
    lcd->print("U");
    return;
  } else {
    lcd->print("D");
  }
  
  
  if (id == 0) {
    value++;
  } else if (id == 1 && value > 0) {
    value--;
  }


  seg->setNum(0, value % 10);
  unsigned int v = (value / 10) % 10;
  seg->setNum(1, v);
  if (value < 10){
    seg->mute(1);
  }
}

