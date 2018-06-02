#include "Symbols.h"
#include <Arduino.h>
#include <Wire.h>
#include "Atlas.h"

const uint8_t puzzleRows[6][7] = {
    {0, 5, 12, 18, 16, 11, 13}, {1, 0, 13, 14, 19, 11, 21},
    {2, 8, 14, 20, 25, 12, 19}, {3, 9, 15, 16, 20, 21, 10},
    {4, 10, 15, 13, 9, 22, 7},  {3, 1, 17, 23, 4, 24, 6}};

Symbols::Symbols(ShiftOut *out, ShiftIn *in, Adafruit_SSD1306 *tft,
                 ButtonManager *buttons, RGB_LED *led, Timer *t,
                 ArduinoGameManager *gameManager)
    : buttonListener(this), timerListener(this) {
  this->out = out;
  this->in = in;
  this->buttons = buttons;
  this->led = led;
  this->tft = tft;
  this->t = t;
  this->gameManager = gameManager;
  this->gameManager->setGameModule(this);
  this->buttons->attachAllOnPress(&buttonListener);
  t->attachTimer(&timerListener, 0);
}

void Symbols::init(uint32_t seed) {
  r.reseed(seed);

  // Get the row that is going to be used to generate the puzzle.
  puzzleRowIndex = r.next() % 6;
  /* Serial.print("Selected Column: "); */
  /* Serial.println(int(puzzleRowIndex)); */

  // Randomly choose the first four possibilities.
  uint8_t tempRow[] = {0, 1, 2, 3, 4, 5, 6};
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t offset = r.next() % (7 - i);
    uint8_t temp = tempRow[i];
    tempRow[i] = tempRow[i + offset];
    tempRow[i + offset] = temp;
  }

  // Sort the array using bubble sort.
  bool sorted = false;
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

  /* Serial.print("Row List: "); */
  /* for (uint8_t i = 0; i < 4; i++) { */
  /*   Serial.print(int(tempRow[i])); */
  /*   Serial.print(" "); */
  /* } */
  /* Serial.println(); */

  // Select the symbols to be used for the puzzle.
  for (uint8_t i = 0; i < 4; i++) {
    symbolList[i] = puzzleRows[puzzleRowIndex][tempRow[i]];
  }

  /* Serial.print("Symbol List: "); */
  /* for (uint8_t i = 0; i < 4; i++) { */
  /*   Serial.print(int(symbolList[i])); */
  /*   Serial.print(" "); */
  /* } */
  /* Serial.println(); */

  // Initialize the button list.
  for (uint8_t i = 0; i < 4; i++) { buttonList[i] = i; }

  // Randomize the button used for the symbols.
  for (uint8_t i = 0; i < 3; i++) {
    uint8_t offset = r.next() % (4 - i);
    uint8_t temp = buttonList[i];
    buttonList[i] = buttonList[i + offset];
    buttonList[i + offset] = temp;
  }

  /* Serial.print("Button Order: "); */
  /* for (uint8_t i = 0; i < 4; i++) { */
  /*   Serial.print(int(buttonList[i])); */
  /*   Serial.print(" "); */
  /* } */
  /* Serial.println(); */

  // Draw the symbols on the I2C displays.
  for (uint8_t i = 0; i < 4; i++) { drawSymbol(buttonList[i], symbolList[i]); }

  // Reset the currentButtonIndex.  This is used to determine if the player
  // has won the game.
  currentButtonIndex = 0;

  // Start the module.
  setMode(ModuleMode::Armed);
}

void Symbols::demo() {
}

void Symbols::start() {
}

void Symbols::explode() {
}

void Symbols::drawSymbol(uint8_t screen, uint8_t symbol) {
  // Validate the screen number.
  if (screen >= 4) { return; }

  // Validate the symbol.
  if (symbol >= 26) { return; }

  uint8_t counter = 0;
  uint8_t result;

  do {
    Wire.beginTransmission(0x70);
    Wire.write(1 << screen);
    result = Wire.endTransmission();
    counter++;
  } while (result != 0 && counter <= 3);

  // Clear the display to black.
  tft->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  tft->clearDisplay();
  tft->display();

  // Calculate the address of the bitmap.
  // Each symbol is 512 bytes long.
  const uint8_t *address = &atlas[512 * symbol];
  tft->drawBitmap(32, 0, address, 64, 64, WHITE);

  // Update the OLED LCD.
  tft->display();
}

void Symbols::setMode(ModuleMode mode) {
  // Store the current mode.
  this->mode = mode;

  switch (mode) {
    case ModuleMode::Armed:
      led->set(0, LED_RED);
      break;
    case ModuleMode::Disarmed:
      led->set(0, LED_GREEN);
      break;
    case ModuleMode::Demo:
      led->set(0, LED_BLUE);
      break;
    default:
      break;
  }
}

Symbols::SymbolsButtonListener::SymbolsButtonListener(Symbols *parent) {
  this->parent = parent;
}

void Symbols::SymbolsButtonListener::onEvent(Button *caller,
                                             ButtonEvent event) {
  (void)(caller);
  (void)(event);

  // Turn on the button LEDS.
  if (event == ButtonEvent::PRESS) {
    parent->out->set(caller->getID(), 0, true);
  } else if (event == ButtonEvent::RELEASE) {
    parent->out->set(caller->getID(), 0, false);
  }

  // Only handle button up events.
  if (event != ButtonEvent::PRESS) { return; }

  switch (parent->mode) {
    case ModuleMode::Armed:
      if (caller->getID() == parent->buttonList[parent->currentButtonIndex]) {
        parent->currentButtonIndex++;

        if (parent->currentButtonIndex >= 4) {
          // Sucessfully disarmed the module.
          parent->setMode(ModuleMode::Disarmed);
          /* Serial.println("Disarmed."); */
        }
      } else {
        /* Serial.println("Strike!"); */
        // Reset the currentButtonIndex.
        parent->currentButtonIndex = 0;
      }
      break;
    default:
      break;
  };
}

Symbols::SymbolsTimerListener::SymbolsTimerListener(Symbols *parent) {
  this->parent = parent;
}

unsigned int Symbols::SymbolsTimerListener::onEvent(Timer *caller,
                                                    unsigned int id,
                                                    unsigned int calls) {
  (void)(caller);
  (void)(id);
  (void)(calls);

  static uint8_t demoCounter = 0;

  // Just change the displays to different symbols over time.
  if (parent->mode == ModuleMode::Demo) {
    parent->drawSymbol(demoCounter % 4, demoCounter % 27);
    demoCounter++;
  }

  return 1500;
}
