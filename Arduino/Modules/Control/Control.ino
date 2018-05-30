#define DATA_OUT_PIN 2
#define OUT_LOAD_PIN 4
#define REG_CLOCK 6
#define DATA_IN_PIN 3
#define IN_LOAD_PIN 5

#include <Button.h>
#include <ButtonManager.h>
#include <GameState.h>
#include <LFSR.h>
#include <LiquidCrystal_I2C.h>
#include <RGB_LED.h>
#include <SPIManager.h>
#include <ShiftIn.h>
#include <ShiftOut.h>
#include <Timer.h>
#include "Adafruit_LEDBackpack.h"
#include "Control.h"

ShiftOut out;
ShiftIn in(REG_CLOCK, IN_LOAD_PIN, DATA_IN_PIN, 1);
RGB_LED led(&out, 5, 0, 6, 0, 7, 0);
ButtonManager buttons(0, 4, &in);
Timer t(1);
ArduinoGameManager gameManager;
LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_7segment ssd;
Control module(&in, &out, &led, &buttons, &t, &gameManager, &lcd, &ssd);

void setup() {
  Serial.begin(115200);
  unsigned int pin = DATA_OUT_PIN;
  out.build(1, 1, &pin, OUT_LOAD_PIN, REG_CLOCK);
  gameManager.getGameState().setPorts(~0);
  gameManager.getGameState().setIndicators(~0);
  // Initialize the LCD display.
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Initialize the seven segment display.
  ssd.begin(0x70);

  // Initialize the module.
  module.init(3968);
}

void loop() {
  out.load();
  in.load();
  buttons.load();
  t.load();
  gameManager.update();
}
