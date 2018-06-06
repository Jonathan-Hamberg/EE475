#include <Adafruit_SSD1306.h>
#include <Button.h>
#include <ButtonManager.h>
#include <GameState.h>
#include <LFSR.h>
#include <RGB_LED.h>
#include <SPIManager.h>
#include <ShiftIn.h>
#include <ShiftOut.h>
#include <Timer.h>
#include <Wire.h>
#include "ArduinoGameManager.h"
#include "Symbols.h"
#define DATA_IN_PIN 4
#define IN_LOAD_PIN 6

#define DATA_OUT_PIN 3
#define OUT_LOAD_PIN 5
#define REG_CLOCK 7

ShiftIn in(REG_CLOCK, IN_LOAD_PIN, DATA_IN_PIN, 1);
ShiftOut out;
Timer t(1);
Adafruit_SSD1306 tft(-1);
ButtonManager buttons(0, 4, &in);
RGB_LED led(&out, 7, 0, 5, 0, 6, 0);
ArduinoGameManager gameManager(ModuleType::Symbols);
Symbols module(&out, &in, &tft, &buttons, &led, &t, &gameManager);

// Un-comment #define SSD1306_128_64 on line 73 in file Adafruit_SSD1306.h
// 128 x 64 pixel display
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {

  // put your setup code here, to run once:
  unsigned int pin = DATA_OUT_PIN;
  out.build(6, 1, &pin, OUT_LOAD_PIN, REG_CLOCK);

  module.init(3968);
}

void loop() {
  out.load();
  in.load();
  buttons.load();
  t.load();
  gameManager.update();
}
