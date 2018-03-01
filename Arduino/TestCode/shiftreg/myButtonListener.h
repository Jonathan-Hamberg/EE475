#ifndef MY_BUTTON_LISTENER_H
#define MY_BUTTON_LISTENER_H

#include <Arduino.h>
#include <Button.h>
#include <RGB_LED.h>

class MyButtonListener : public ButtonListener {
  public:

  MyButtonListener(RGB_LED *led, unsigned int n_up, unsigned int n_down);

  virtual void onEvent(Button* caller, ButtonEvent event);


  private:
  RGB_LED *led;
  unsigned int n_up, n_down;
};

#endif // MY_BUTTON_LISTENER_H
