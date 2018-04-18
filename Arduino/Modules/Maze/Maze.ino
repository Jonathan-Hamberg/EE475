#define DATA_OUT_PIN 3
#define OUT_LOAD_PIN 4
#define REG_CLOCK 5
#define DATA_IN_PIN 7
#define IN_LOAD_PIN 8
#define NEO_PIN 13

#include <ShiftOut.h>
#include <ShiftIn.h>
#include <RGB_LED.h>
#include <Button.h>
#include <ButtonManager.h>
#include <LFSR.h>
#include <Timer.h>
#include <GameState.h>
#include <Adafruit_NeoPixel.h>
#include "Maze.h"

ShiftOut out;
ShiftIn in(REG_CLOCK, IN_LOAD_PIN, DATA_IN_PIN, 1);
RGB_LED led(&out, 16, 0, 8, 0, 0, 0);
ButtonManager buttons(0, 4, &in);
Timer t(8);
GameState game;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, NEO_PIN, NEO_GRB + NEO_KHZ800);
Maze module(&out, &buttons, &t, &led, &strip, &game);


void setup() {
  // put your setup code here, to run once:
  unsigned int pin = DATA_OUT_PIN;
  strip.begin();
  strip.show();
  out.build(6, 1, &pin, OUT_LOAD_PIN, REG_CLOCK);
  module.init(analogRead(1));
}

void loop() {
  // put your main code here, to run repeatedly:

}
