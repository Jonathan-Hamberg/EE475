#define DATA_OUT_PIN 3
#define OUT_LOAD_PIN 4
#define REG_CLOCK 5
#define DATA_IN_PIN 7
#define IN_LOAD_PIN 8

#define GREEN_PIN 2
#define BLUE_PIN 3
#define RED_PIN 6
#define RCLK_PIN 4
#define SRCLK_PIN 5
#define REG_CLOCK 5
#define DATA_PIN 7
#define LOAD_PIN 8

#define RS 2
#define EN 6
#define D7 12
#define D6 11
#define D5 10
#define D4 9
#define NEO_PIN 13

#include <ShiftOut.h>
#include <ShiftIn.h>
#include <RGB_LED.h>
#include <Button.h>
#include <ButtonManager.h>
#include <LFSR.h>
#include <Timer.h>
#include <GameState.h>
#include <SPIManager.h>
#include <Adafruit_LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include "TheButton.h"

ShiftOut out;
ShiftIn in(REG_CLOCK, LOAD_PIN, DATA_PIN, 1);
RGB_LED led(&out, 16, 0, 8, 0, 0, 0);
ButtonManager buttons(0, 4, &in);
Timer t(8);
GameState game;
SPIManager spiManager(&game);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, NEO_PIN, NEO_GRB + NEO_KHZ800);
//LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
LiquidCrystal_I2C lcd(0x3F,16,2);
TheButton module(&led, buttons.getButton(0), &t, &game, &spiManager, &lcd, &strip);

void setup() {
  Serial.begin(9600);
  unsigned int pin = DATA_OUT_PIN;
  strip.begin();
  strip.show();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  out.build(6, 1, &pin, RCLK_PIN, SRCLK_PIN);
  module.init(analogRead(1));
  module.start();
}

void loop() {
  out.load();
  in.load();
  buttons.load();
  t.load();
}
