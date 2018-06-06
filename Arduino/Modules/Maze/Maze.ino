#define DATA_OUT_PIN 2
#define OUT_LOAD_PIN 4
#define REG_CLOCK 6
#define DATA_IN_PIN 3
#define IN_LOAD_PIN 5
#define NEO_PIN 7

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
RGB_LED led(&out, 4, 0, 6, 0, 7, 0);
ButtonManager buttons(0, 4, &in);
Timer t(8);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(36, NEO_PIN, NEO_GRB + NEO_KHZ800);
ArduinoGameManager gameManager(ModuleType::Mazes);
Maze module(&out, &buttons, &t, &led, &strip, &gameManager);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  unsigned int pin = DATA_OUT_PIN;
  strip.begin();
  strip.show();
  out.build(6, 1, &pin, OUT_LOAD_PIN, REG_CLOCK);
  module.init(3968);
}

void loop() {
  // put your main code here, to run repeatedly:
  out.load();
  in.load();
  buttons.load();
  t.load();

  gameManager.update();
}
