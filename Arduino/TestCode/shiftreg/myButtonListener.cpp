#include <Arduino.h>
#include "myButtonListener.h"

MyButtonListener::MyButtonListener(RGB_LED *led, unsigned int n_up, unsigned int n_down) {
  this->led = led;
  this->n_up = n_up;
  this->n_down = n_down;
}

void MyButtonListener::onEvent(Button* caller, ButtonEvent event) {
  if (event == ButtonEvent::PRESS) {
    led->set(caller->getID(), LED_GREEN);
    if (caller->getID() == 1) {
      Serial.print("BOOM!!\n");
    }
  } else if (event == ButtonEvent::RELESE) {
    led->set(caller->getID(), LED_RED);
  }
}

