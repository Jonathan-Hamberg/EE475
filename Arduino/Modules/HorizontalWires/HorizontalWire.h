#ifndef HORIZONTAL_WIRE_H
#define HORIZONTAL_WIRE_H

#include <Arduino.h>
#include <ArduinoGameManager.h>
#include <Button.h>
#include <ButtonManager.h>
#include <GameModule.h>
#include <GameState.h>
#include <LFSR.h>
#include <RGB_LED.h>
#include <ShiftIn.h>
#include <ShiftOut.h>

#define HORIZONTAL_WIRE_LENGTH 6
#define HORIZONTAL_WIRE_INDEX_OFFSET 0

class HorizontalWire : public GameModule {
 public:
  HorizontalWire(ShiftIn* in, ShiftOut* out, RGB_LED* led,
                 ButtonManager* buttons, ArduinoGameManager* gameManager);

  virtual void init(uint32_t seed);

  virtual void start();

  virtual void demo();

  virtual void explode();

  virtual ~HorizontalWire() = default;

 private:
  void setMode(ModuleMode mode);
  void muteOutput();
  void updateColors();
  void updateIO();
  void findTarget();

  class HorizontalWireButtonListener : public ButtonListener {
   public:
    HorizontalWireButtonListener(HorizontalWire* parent);

    virtual void onEvent(Button* caller, ButtonEvent event);

   private:
    HorizontalWire* parent;
  };

  HorizontalWireButtonListener buttonListener;

  ShiftIn* in;
  ShiftOut* out;
  RGB_LED* led;
  ButtonManager* buttons;
  GameState* game;
  ArduinoGameManager* gameManager;
  lfsr r;
  uint8_t removedWires;

  ModuleMode mode;
  uint8_t numWires;
  int8_t target;
  uint8_t colors[HORIZONTAL_WIRE_LENGTH];
  int8_t wireMap[HORIZONTAL_WIRE_LENGTH];
  int8_t wireIndex[HORIZONTAL_WIRE_LENGTH];
};

#endif  // HORIZONTAL_WIRE_H
