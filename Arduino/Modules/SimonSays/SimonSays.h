#ifndef SIMON_SAYS_H
#define SIMON_SAYS_H

#include <Arduino.h>
#include <RGB_LED.h>
#include <ShiftIn.h>
#include <ShiftOut.h>
#include <Button.h>
#include <ButtonManager.h>
#include <LFSR.h>
#include <Timer.h>
#include <GameState.h>
#include "GameModule.h"

#define RED_INDEX 0
#define BLUE_INDEX 1
#define YELLOW_INDEX 3
#define GREEN_INDEX 2

#define SIMON_SAYS_LENGTH 6

class SimonSays : public GameModule {
  public:

  SimonSays(ShiftIn * in, ShiftOut * out, RGB_LED * led, ButtonManager * buttons, Timer * t, GameState * game);

  virtual void init(uint32_t seed);

  virtual void start();

  virtual void demo();

  virtual void explode();

  virtual ~SimonSays();

  private:

  class SimonSaysButtonListener : public ButtonListener {
    public:
    SimonSaysButtonListener(SimonSays *parent);

    virtual void onEvent(Button * caller, ButtonEvent event);
    
    private:

    bool setButtons();

    void armedAction(Button * caller, ButtonEvent event);

    void demoAction(Button * caller, ButtonEvent event);

    void disarmedAction(Button * caller, ButtonEvent event);

    void offAction(Button * caller, ButtonEvent event);
    
    SimonSays *parent;
  };

  class SimonSaysTimerListener : public TimerListener {
    public:
    SimonSaysTimerListener(SimonSays *parent);

    virtual unsigned int onEvent(Timer * caller, unsigned int id, unsigned int calls);

    private:
    SimonSays *parent;
  };

  void setMode(uint8_t mode);
  void muteOutput();

  SimonSaysButtonListener buttonListener;
  SimonSaysTimerListener timerListener;

  ShiftIn * in;
  ShiftOut * out;
  RGB_LED * led;
  ButtonManager * buttons;
  Timer * t;
  GameState * game;
  lfsr r;

  uint8_t colors[SIMON_SAYS_LENGTH];
  uint8_t mode;
  uint8_t count;
  uint8_t count2;
  uint8_t displayCount;

  Timer::timer * currentTimer;
  
};

#endif // SIMON_SAYS_H
