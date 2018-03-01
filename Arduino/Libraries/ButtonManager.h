#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include <Arduino.h>
#include <ShiftIn.h>
#include <Button.h>

class ButtonManager final {
  public:

  ButtonManager(unsigned int start, unsigned int n, ShiftIn* in);

  void attachAllOnPress(ButtonListener *listener);
  void attachAllOnRelease(ButtonListener *listener);

  void detachAllOnPress();
  void detachAllOnRelease();

  Button* getButton(unsigned int n);

  void load();

  ~ButtonManager();

  private:
  unsigned int start, n;
  Button *buttons;
  ShiftIn *in;
  
};

#endif // BUTTON_MANAGER_H
