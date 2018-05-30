#ifndef MAZE_H
#define MAZE_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ArduinoGameManager.h>
#include <ButtonManager.h>
#include <Definitions.h>
#include <GameModule.h>
#include <GameState.h>
#include <LFSR.h>
#include <RGB_LED.h>
#include <ShiftOut.h>
#include <Timer.h>

#include "Atlas.h"

class Symbols : public GameModule {
 public:
  Symbols(ShiftOut *out, ShiftIn *in, Adafruit_SSD1306 *tft,
          ButtonManager *buttons, RGB_LED *led, Timer* t,
          ArduinoGameManager *gameManager);

  virtual void init(uint32_t seed);

  virtual void demo();

  virtual void start();

  virtual void explode();

  void drawSymbol(uint8_t screen, uint8_t symbol);

 private:
  class SymbolsButtonListener : public ButtonListener {
   public:
    SymbolsButtonListener(Symbols *parent);

    virtual void onEvent(Button *caller, ButtonEvent event);

   private:
    Symbols *parent;
  };

  class SymbolsTimerListener : public TimerListener {
   public:
    SymbolsTimerListener(Symbols *parent);

    virtual unsigned int onEvent(Timer *caller, unsigned int id,
                                 unsigned int calls);

   private:
    Symbols *parent;
  };
  /**
   *
   *
   * @param mode
   */
  void setMode(ModuleMode mode);

  // Used to detect button presses.
  SymbolsButtonListener buttonListener;
  // Used by the demo mode to draw symbols in a pattern.
  SymbolsTimerListener timerListener;
  Timer *t;
  lfsr r;
  ArduinoGameManager *gameManager;
  ButtonManager *buttons;
  RGB_LED *led;
  ShiftOut *out;
  ShiftIn *in;
  Adafruit_SSD1306 *tft;
  uint8_t symbolList[4];
  uint8_t buttonList[4];
  uint8_t puzzleRowIndex;
  uint8_t currentButtonIndex;

  ModuleMode mode;
};

#endif  // MAZE_H
