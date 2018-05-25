#include "Symbols.h"
#include <Wire.h>
#include <Arduino.h>
#include "Atlas.h"

const uint8_t puzzleRows[6][7] = {
    {0, 5, 12, 18, 16, 11, 13}, {1, 0, 13, 14, 19, 11, 21},
    {2, 8, 14, 20, 25, 12, 19}, {3, 9, 15, 16, 20, 21, 10},
    {4, 10, 15, 13, 9, 22, 7},  {3, 1, 17, 23, 4, 24, 6}};

Symbols::Symbols(ShiftOut *out, ShiftIn *in, Adafruit_SSD1306 *tft,
                 ButtonManager *buttons, RGB_LED *led, GameState *game)
    : buttonListener(this) {
  this->out = out;
  this->in = in;
  this->buttons = buttons;
  this->led = led;
  this->game = game;
  this->tft = tft;
  this->buttons->attachAllOnPress(&buttonListener);
  this->buttons->attachAllOnRelease(&buttonListener);
}

void Symbols::init(uint32_t seed) {
  r.reseed(seed);

  drawSymbol(0, 0);
  drawSymbol(1, 1);
  drawSymbol(2, 2);
  drawSymbol(3, 3);

  return;

  // Get the row that is going to be used to generate the puzzle.
  puzzleRowIndex = r.next() % 6;

  // Randomly choose the first four possibilities.
  uint8_t tempRow[] = {0, 1, 2, 3, 4, 5, 6};
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t offset = r.next() % (7 - i);
    uint8_t temp = tempRow[i];
    tempRow[i] = tempRow[i + offset];
    tempRow[i + offset] = temp;
  }

  // Sort the array using bubble sort.
  bool sorted = true;
  while (!sorted) {
    sorted = true;
    for (uint8_t i = 0; i < 3; i++) {
      if (tempRow[i] > tempRow[i + 1]) {
        uint8_t temp = tempRow[i];
        tempRow[i] = tempRow[i + 1];
        tempRow[i + 1] = temp;
        sorted = false;
      }
    }
  }

  // Select the symbols to be used for the puzzle.
  for (uint8_t i = 0; i < 4; i++) {
    symbolList[i] = puzzleRows[puzzleRowIndex][tempRow[i]];
  }

  // Initialize the button list.
  for (uint8_t i = 0; i < 4; i++) { buttonList[i] = i; }

  // Randomize the button used for the symbols.
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t offset = r.next() % (7 - i);
    uint8_t temp = buttonList[i];
    buttonList[i] = buttonList[i + offset];
    buttonList[i + offset] = temp;
  }

  // Draw the symbols on the I2C displays.
  for (uint8_t i = 0; i < 4; i++) {
    drawSymbol(buttonList[i], symbolList[buttonList[i]]);
  }

  // Reset the currentButtonIndex.  This is used to determine if the player
  // has won the game.
  currentButtonIndex = 0;
}

void Symbols::demo() {}

void Symbols::start() {}

void Symbols::explode() {}

void Symbols::drawSymbol(uint8_t screen, uint8_t symbol) {
  // Validate the screen number.
  if (screen >= 4) { return; }

  // Validate the symbol.
  if (symbol >= 26) { return; }

  Wire.beginTransmission(0x70);
  Wire.write(1 << screen);
  Wire.endTransmission();

  // Fill the screen to white.
  tft->fillRect(0, 0, 128, 64, BLACK);

  // Calculate the address of the bitmap.
  // Each symbol is 512 bytes long.
  const uint8_t *address = &atlas[512 * symbol];
  tft->drawBitmap(32, 0, address, 64, 64, WHITE);

  // Update the OLED LCD.
  tft->display();
}

Symbols::SymbolsButtonListener::SymbolsButtonListener(Symbols *parent) {
  this->parent = parent;
}

void Symbols::SymbolsButtonListener::onEvent(Button *caller,
                                             ButtonEvent event) {
  (void)(caller);
  (void)(event);

  Serial.print("Button ");
  Serial.print(caller->getID());
  Serial.println(" Pressed.");
}
