#ifndef THE_BUTTON_H
#define THE_BUTTON_H

#include <Arduino.h>
#include <GameModule.h>
#include <ShiftOut.h>
#include <RGB_LED.h>
#include <Button.h>
#include <LFSR.h>
#include <GameState.h>
#include <SPIManager.h>
#include <Timer.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

#define NEO_RED 131072
#define NEO_GREEN 256
#define NEO_BLUE 1
#define NEO_YELLOW 131328
#define NEO_WHITE 131329

class TheButton : public GameModule {
  public:

  TheButton(RGB_LED * led, Button * button, Timer * t, GameState * game, SPIManager* spiManager, LiquidCrystal_I2C * lcd, Adafruit_NeoPixel * strip);

  virtual void init(uint32_t seed);

  virtual void start();

  virtual void demo();

  virtual void explode();

  virtual ~TheButton();
  
  private:

  void muteOutput();

  void setMode(ModuleMode mode);

  void setColor(uint32_t color);

  bool quickPress();

  uint32_t buttonColor;
  uint32_t downColor;

  ModuleMode mode;
  uint8_t textIndex;

  char displayTxt[17];

  lfsr r;
  GameState * game;
  RGB_LED * led;
  Button * button;
  Timer * t;
  Timer::timer * currentTimer;
  LiquidCrystal_I2C * lcd;
  Adafruit_NeoPixel * strip;
  SPIManager* spiManager;

  class TheButtonButtonListener : public ButtonListener {
    public:

    TheButtonButtonListener(TheButton * parent);

    virtual void onEvent(Button * caller, ButtonEvent event);
    
    private:
    TheButton * parent;
  };

  class TheButtonTimerListener : public TimerListener {
    public:

    TheButtonTimerListener(TheButton * parent);

    virtual unsigned int onEvent(Timer *caller, unsigned int id, unsigned int calls);
    
    private:
    TheButton * parent;
  };

  TheButtonButtonListener buttonListener;
  TheButtonTimerListener timerListener;
};

#endif // THE_BUTTON_H
