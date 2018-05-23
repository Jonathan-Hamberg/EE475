#ifndef PASSWORD_H
#define PASSWORD_H

#include <Arduino.h>
#include <GameModule.h>
#include <ShiftOut.h>
#include <RGB_LED.h>
#include <ButtonManager.h>
#include <LFSR.h>
#include <SPIManager.h>
#include "PasswordContainer.h"

class Password : public GameModule {
  public:

  Password(ShiftOut * out, RGB_LED * led, ButtonManager * buttons, GameState * game, SPIManager * spiManager);

  virtual void init(uint32_t seed);

  virtual void start();

  virtual void demo();

  virtual void explode();
  
  private:

  void muteOutput();

  void setMode(MoudleMode mode);

  ModuleMode mode;

  lfsr r;
  GmaeState * game;
  RGB_LED * led;
  ButtonManager * buttons;
  SiftOut * out;
  SPIManager * spiManager;
  PasswordContainer password;
};

#endif // PASSWORD_H
