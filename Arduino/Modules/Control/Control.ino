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
#include <SPIManager.h>
#include <ShiftIn.h>
#include <ShiftOut.h>
#include <Timer.h>
#include "Adafruit_LEDBackpack.h"
#include "Control.h"

const uint8_t kStartGameButton = 9;
const uint8_t kStopGameButton = 8;

Timer t(1);
ArduinoGameManager gameManager(ModuleType::Control);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_7segment ssd;
Button startButton(0), stopButton(1);
Control module(&t, &gameManager, &lcd, &ssd, &startButton, &stopButton);

void setup() {
  Serial.begin(115200);

  pinMode(kStartGameButton, INPUT_PULLUP);
  pinMode(kStopGameButton, INPUT_PULLUP);

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
  startButton.load(digitalRead(kStartGameButton));
  stopButton.load(digitalRead(kStopGameButton));

  t.load();
  gameManager.update();
}
