#define DATA_OUT_PIN 2
#define OUT_LOAD_PIN 4
#define REG_CLOCK 6
#define DATA_IN_PIN 3
#define IN_LOAD_PIN 5

#include <ArduinoGameManager.h>
#include <Button.h>
#include <ButtonManager.h>
#include <GameState.h>
#include <LFSR.h>
#include <ShiftIn.h>
#include <ShiftOut.h>
#include "HorizontalWire.h"

ShiftOut out;
ShiftIn in(REG_CLOCK, IN_LOAD_PIN, DATA_IN_PIN, 1);
RGB_LED led(&out, 25, 0, 17, 0, 9, 0);
ButtonManager buttons(0, 6, &in);
ArduinoGameManager gameManager(ModuleType::Wires);
HorizontalWire module(&in, &out, &led, &buttons, &gameManager);

uint8_t loadCounter = 0;
void setup() {
  Serial.begin(115200);
  unsigned int pin = DATA_OUT_PIN;
  out.build(4, 1, &pin, OUT_LOAD_PIN, REG_CLOCK);

  module.init(3968);
}

void loop() {
  out.load();
  in.load();

  // Only update the buttons every four loops to debounce the input.
  if (loadCounter % 4 == 0) {
    buttons.load();
  }

  gameManager.update();
}
