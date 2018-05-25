#include <Adafruit_GFX.h>
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
#include "Symbols.h"
extern "C" {
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}
#define DATA_IN_PIN 4
#define IN_LOAD_PIN 6

#define DATA_OUT_PIN 3
#define OUT_LOAD_PIN 5
#define REG_CLOCK 7
;
ShiftIn in(REG_CLOCK, IN_LOAD_PIN, DATA_IN_PIN, 1);
ShiftOut out;
Adafruit_SSD1306 tft(-1);
ButtonManager buttons(4, 4, &in);
GameState game;
RGB_LED led(&out, 16, 0, 8, 0, 0, 0);
SPIManager spiManger(&game);
Symbols module(&out, &in, &tft, &buttons, &led, &game);

// Un-comment #define SSD1306_128_64 on line 73 in file Adafruit_SSD1306.h
// 128 x 64 pixel display
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
#define TCAADDR 0x70

void setup() {
  // Assign the type to the module.
  spiManger.setModuleType(ModuleType::Symbols);

  // put your setup code here, to run once:
  unsigned int pin = DATA_OUT_PIN;
  out.build(6, 1, &pin, OUT_LOAD_PIN, REG_CLOCK);

  for (uint8_t i = 0; i < 4; i++) {
// Select the correct I2C mux port.
    Wire.beginTransmission(0x70);
    Wire.write(1 << i);
    Wire.endTransmission();

    // Initialize with the I2C addr 0x3C (for the 128x64).
    tft.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    tft.clearDisplay();
    tft.display();
  }

  module.init(analogRead(1));
}

void loop() {
  // put your main code here, to run repeatedly:
  out.load();
  in.load();
  buttons.load();
}
