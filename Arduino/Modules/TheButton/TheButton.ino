#define DATA_OUT_PIN 2
#define OUT_LOAD_PIN 4
#define REG_CLOCK 6
#define NEO_PIN 3
#define BUTTON_PIN 5

#include <ShiftOut.h>
#include <RGB_LED.h>
#include <Button.h>
#include <LFSR.h>
#include <Timer.h>
#include <ArduinoGameManager.h>
#include <Adafruit_LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include "TheButton.h"

ShiftOut out;
RGB_LED led(&out, 6, 0, 4, 0, 3, 0);
Button button;
Timer t(8);
ArduinoGameManager gameManager(ModuleType::Button);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, NEO_PIN, NEO_GRB + NEO_KHZ800);
//LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
LiquidCrystal_I2C lcd(0x3F,16,2);
TheButton module(&led, &button, &t, &lcd, &strip, &gameManager);

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  unsigned int pin = DATA_OUT_PIN;
  strip.begin();
  strip.show();
  lcd.init();
  lcd.noBacklight();
  lcd.clear();
  out.build(6, 1, &pin, OUT_LOAD_PIN, REG_CLOCK);
  module.init(analogRead(1));
  //module.start();
}

void loop() {
  static unsigned int count = 0;
  out.load();
  if (count % 8 == 0) {
    button.load(!digitalRead(BUTTON_PIN));
  }
  t.load();
  gameManager.update();
  count++;
}
