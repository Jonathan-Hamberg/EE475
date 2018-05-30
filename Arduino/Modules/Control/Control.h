#ifndef CONTROL_H
#define CONTROL_H

#include <Adafruit_LEDBackpack.h>
#include <Arduino.h>
#include <ArduinoGameManager.h>
#include <Button.h>
#include <ButtonManager.h>
#include <GameModule.h>
#include <GameState.h>
#include <LFSR.h>
#include <LiquidCrystal_I2C.h>
#include <RGB_LED.h>
#include <ShiftIn.h>
#include <ShiftOut.h>
#include <Timer.h>

#define RED_INDEX 0
#define BLUE_INDEX 1
#define YELLOW_INDEX 3
#define GREEN_INDEX 2

/**
 * The Control module is special in the fact that it is not a solvable module,
 * but is used to display important information to the user.  It displays the
 * following information.
 * The countdown timer seven segment displays.
 * The indicators that are present on the bomb.
 * The connectors that are present on the bomb.
 * The number of strikes that have occured on the bomb.
 * The bomb state.  eg. Off, Demo, Armed, Defused, Exploded
 */
class Control : public GameModule {
 public:
  Control(ShiftIn *in, ShiftOut *out, RGB_LED *led, ButtonManager *buttons,
          Timer *t, ArduinoGameManager *gameManager, LiquidCrystal_I2C *lcd,
          Adafruit_7segment *ssd);

  virtual void init(uint32_t seed);

  virtual void start();

  virtual void demo();

  virtual void explode();

  virtual ~Control();

 private:
  /**
   * This class is used to listen for the start and stop game buttons.
   */
  class ControlButtonListener : public ButtonListener {
   public:
    ControlButtonListener(Control *parent);

    virtual void onEvent(Button *caller, ButtonEvent event);

   private:
    Control *parent;
  };

  /**
   * This timer listener class is used to display the current countdown of the
   * module.
   */
  class ControlTimerListener : public TimerListener {
   public:
    ControlTimerListener(Control *parent);

    virtual unsigned int onEvent(Timer *caller, unsigned int id,
                                 unsigned int calls);

   private:
    Control *parent;
  };

  void setMode(ModuleMode mode);
  void muteOutput();

  ControlButtonListener buttonListener;
  ControlTimerListener timerListener;

  ShiftIn *in;
  ShiftOut *out;
  RGB_LED *led;
  ButtonManager *buttons;
  ArduinoGameManager *gameManager;
  GameState *gameState;
  Timer *t;
  Timer::timer *currentTimer;
  LiquidCrystal_I2C *lcd;
  Adafruit_7segment *ssd;

  // Used to store the text displayed for the indicators.
  char indicatorText[3][21];
  uint8_t indicatorRows;

  // Used to store the text displayed for the connectors.
  char portText[2][21];
  uint8_t portRows;

  ModuleMode mode;
};

#endif  // CONTROL_H
