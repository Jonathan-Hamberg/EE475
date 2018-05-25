#ifndef MAZE_H
#define MAZE_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ButtonManager.h>
#include <GameModule.h>
#include <GameState.h>
#include <LFSR.h>
#include <RGB_LED.h>
#include <ShiftOut.h>
#include "Atlas.h"

class Symbols : public GameModule {
 public:
  Symbols(ShiftOut *out, ShiftIn* in, Adafruit_SSD1306* tft, ButtonManager *buttons, RGB_LED *led,
          GameState *game);

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

  SymbolsButtonListener buttonListener;
  lfsr r;
  GameState *game;
  ButtonManager *buttons;
  RGB_LED *led;
  ShiftOut *out;
  ShiftIn *in;
  Adafruit_SSD1306* tft;
  uint8_t symbolList[4];
  uint8_t buttonList[4];
  uint8_t puzzleRowIndex;
  uint8_t currentButtonIndex;

  uint8_t mode;
};

#endif  // MAZE_H
