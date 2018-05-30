#define DATA_OUT_PIN 2
#define OUT_LOAD_PIN 4
#define REG_CLOCK 6
#define DATA_IN_PIN 3
#define IN_LOAD_PIN 5

#include <ShiftOut.h>
#include <ShiftIn.h>
#include <RGB_LED.h>
#include <Button.h>
#include <ButtonManager.h>
#include <LFSR.h>
#include <Timer.h>
#include <GameState.h>
#include <SPIManager.h>
#include "SimonSays.h"

ShiftOut out;
ShiftIn in(REG_CLOCK, IN_LOAD_PIN, DATA_IN_PIN, 1);
RGB_LED led(&out, 5, 0, 6, 0, 7, 0);
ButtonManager buttons(0, 4, &in);
Timer t(1);
GameState game;
SPIManager spiManager(&game);
SimonSays module(&in, &out, &led, &buttons, &t, &game, &spiManager);

void setup() {
  unsigned int pin = DATA_OUT_PIN;
  out.build(1, 1, &pin, OUT_LOAD_PIN, REG_CLOCK);
  game.init(300, 3, 3968);
  module.init(analogRead(1));
  module.start();
}

void loop() {
  out.load();
  in.load();
  buttons.load();
  t.load();
  delay(10);
}
