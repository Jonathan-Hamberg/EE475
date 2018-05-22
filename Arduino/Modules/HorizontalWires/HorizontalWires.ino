#define DATA_OUT_PIN 2
#define OUT_LOAD_PIN 4
#define REG_CLOCK 6
#define DATA_IN_PIN 3
#define IN_LOAD_PIN 5

#include <ShiftOut.h>
#include <ShiftIn.h>
#include <Button.h>
#include <ButtonManager.h>
#include <LFSR.h>
#include <GameState.h>
#include "HorizontalWire.h"

ShiftOut out;
ShiftIn in(REG_CLOCK, IN_LOAD_PIN, DATA_IN_PIN, 1);
RGB_LED led(&out, 25, 0, 17, 0, 9, 0);
ButtonManager buttons(0, 6, &in);
GameState game;
HorizontalWire module(&in, &out, &led, &buttons, &game);

void setup() {
  Serial.begin(9600);
  unsigned int pin = DATA_OUT_PIN;
  out.build(4, 1, &pin, OUT_LOAD_PIN, REG_CLOCK);
  game.init(300, 3, 3968);
  module.init(analogRead(1));
  module.start();

}

void loop() {
  out.load();
  in.load();
  buttons.load();
}
