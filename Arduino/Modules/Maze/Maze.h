#ifndef MAZE_H
#define MAZE_H

#include <Arduino.h>
#include <GameModule.h>
#include <Adafruit_NeoPixel.h>
#include <ButtonManager.h>
#include <RGB_LED.h>
#include <Timer.h>
#include <GameState.h>
#include <LFSR.h>
#include <ShiftOut.h>
#include "MazeBoard.h"

class Maze : public GameModule {
  public:

  Maze(ShiftOut * out, ButtonManager * buttons, Timer * t, RGB_LED * led, Adafruit_NeoPixel * strip, GameState * game);

  virtual void init(uint32_t seed);

  virtual void demo();

  virtual void start();

  virtual void explode();

  private:

  void updateDisplay(unsigned int cycle);

  void setPoint(uint32_t color, uint8_t x, uint8_t y);

  inline bool checkPoints(MazeBoard::point a, MazeBoard::point b);

  void muteOutput();

  class MazeButtonListener: public ButtonListener {
    public:

    MazeButtonListener(Maze * parent);
    
    virtual void onEvent(Button * caller, ButtonEvent event);

    private:
    Maze * parent;
  };

  class MazeTimerListener: public TimerListener {
    public:

    MazeTimerListener(Maze * parent);

    virtual unsigned int onEvent(Timer *caller, unsigned int id, unsigned int calls);
    
    private:
    Maze * parent;
  };

  MazeButtonListener buttonListener;
  MazeTimerListener timerListener;
  Timer::timer * currentTimer;
  lfsr r;
  MazeBoard board;
  GameState * game;
  ButtonManager * buttons;
  RGB_LED * led;
  Timer * t;
  ShiftOut * out;
  Adafruit_NeoPixel * strip;

  uint8_t mode;
};

#endif // MAZE_H
