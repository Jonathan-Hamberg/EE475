#include "Control.h"
#include <Arduino.h>

Control::Control(Timer* t, ArduinoGameManager* gameManager,
                 LiquidCrystal_I2C* lcd, Adafruit_7segment* ssd,
                 Button* startButton, Button* stopButton)
    : buttonListener(this), timerListener(this) {
  this->t = t;
  this->lcd = lcd;
  this->ssd = ssd;
  this->currentTimer = nullptr;
  this->gameManager = gameManager;
  this->gameManager->setGameModule(this);
  this->gameState = &gameManager->getGameState();
  this->updateText = true;

  startButton->attachOnPress(&buttonListener);
  stopButton->attachOnPress(&buttonListener);
}

void Control::init(uint32_t /*seed*/) {
  muteOutput();
  setMode(ModuleMode::Off);

  // Initialize the timer object.
  this->t->detachTimer(currentTimer);
  currentTimer = this->t->attachTimer(&timerListener, 0);

  // Reset each of the lines to be empty by setting the first character to be
  // null.
  for (uint8_t i = 0; i < 3; i++) {
    indicatorText[i][0] = '\0';
  }

  // Used to keep track of the number of rows needed to dispay all the
  // indicators.
  indicatorRows = 0;
  // Used to store the length of text on the particular row line.
  uint8_t rowLength = 0;
  // Indicators stores the bitfields with the values of the indicators.
  uint16_t indicators = gameState->getIndicators();
  for (uint8_t i = 0; i < uint8_t(GameIndicator::EnumSize); i++) {
    // Determine if the indicator is set.
    if ((indicators & 0x01) == 1) {
      uint8_t textLength = strlen(GameIndicatorText[i]);

      // Determine if the text needs to be put on the next line.
      if (rowLength + textLength > 20) {
        indicatorRows++;
        rowLength = 0;
      }

      // Concatinate the indicator text to the display line.
      strncat(indicatorText[indicatorRows], GameIndicatorText[i],
              20 - rowLength);

      // Update the row length.
      rowLength += textLength;
    }

    // Shifts the bits into the least significast postion.
    indicators >>= 1;
  }

  // Used to keep track of the number of rows needed to dispay all the
  // ports.
  portRows = 0;
  // Reset each of the lines to be empty by setting the first character to be
  // null.
  for (uint8_t i = 0; i < 2; i++) {
    portText[i][0] = '\0';
  }
  // Used to store the length of text on the particular row line.
  rowLength = 0;
  // Ports stores the bitfields with the values of the indicators.
  uint16_t ports = gameState->getPorts();
  for (uint8_t i = 0; i < uint8_t(GamePort::EnumSize); i++) {
    // Determine if the indicator is set.
    if ((ports & 0x01) == 1) {
      uint8_t textLength = strlen(GamePortText[i]);

      // Determine if the text needs to be put on the next line.
      if (rowLength + textLength > 20) {
        portRows++;
        rowLength = 0;
      }

      // Concatinate the indicator text to the display line.
      strncat(portText[portRows], GamePortText[i], 20 - rowLength);

      // Update the row length.
      rowLength += textLength;
    }

    // Shifts the bits into the least significast postion.
    ports >>= 1;
  }

  // Convert max index to size for indicator rows.
  portRows++;
  indicatorRows++;

      // Clear the seven segment display text.
      ssd->writeDigitNum(0, 0);
      ssd->writeDigitNum(1, 0);
      ssd->writeDigitNum(3, 0);
      ssd->writeDigitNum(4, 0);
      ssd->writeColon();
      ssd->writeDisplay();

      // Cleare the lcd port and indicator information.
      lcd->setCursor(0, 1);
      lcd->print("                    ");
      lcd->setCursor(0, 2);
      lcd->print("                    ");
      lcd->setCursor(0, 3);
      lcd->print("SN:        Bat:     ");
}

void Control::start() {
  Serial.println("start()...");
  muteOutput();
  setMode(ModuleMode::Armed);
  // Cleare the line.
  lcd->setCursor(0, 3);
  // Display the serial number on the lcd.
  lcd->print("SN: ");
  lcd->print(gameState->getSN());
  // Display the number of batteries on the lcd.
  lcd->print(" Bat: ");
  lcd->print(gameState->getBatteries());

  // Tell the scd to update the indicator and port text.
  updateText = true;
}

void Control::demo() {
  Serial.println("demo()...");
  muteOutput();
  setMode(ModuleMode::Demo);

  updateText = true;
}

void Control::explode() {
  Serial.println("explode()...");
  muteOutput();
  setMode(ModuleMode::Off);

  updateText = true;
}

void Control::setMode(ModuleMode mode) {
  // Select the first line of the LCD.
  lcd->setCursor(0, 0);
  lcd->print("State: ");
  this->mode = mode;
  switch (mode) {
    case ModuleMode::Off:
      lcd->print("Off                 ");
      break;
    case ModuleMode::Armed:
      lcd->print("Armed               ");
      break;
    case ModuleMode::Disarmed:
      lcd->print("Disarmed            ");
      break;
    case ModuleMode::Demo:
      lcd->print("Demo                ");
      break;
    default:
      break;
  }
}

void Control::muteOutput() {
}

Control::ControlButtonListener::ControlButtonListener(Control* parent) {
  this->parent = parent;
}

void Control::ControlButtonListener::onEvent(Button* caller,
                                             ButtonEvent /*event*/) {
  if (caller->getID() == 0) {
    Serial.println("startGame button...");
    parent->gameManager->startGame();
  } else if (caller->getID() == 1) {
    Serial.println("stopGame button...");
    parent->gameManager->stopGame();
  }
}

Control::ControlTimerListener::ControlTimerListener(Control* parent) {
  this->parent = parent;
}

unsigned int Control::ControlTimerListener::onEvent(Timer* /*caller*/,
                                                    unsigned int /*id*/,
                                                    unsigned int /*calls*/) {
  static uint32_t counter = 0;
  static uint32_t displayCounter = 0;
  static uint32_t countdown = 0;

  // Draw the countdown timer if necessary.
  if (countdown != parent->gameState->getCountdownTime()) {
    countdown = parent->gameState->getCountdownTime();
    uint8_t seconds = countdown % 60;
    uint8_t minutes = countdown / 60;

    // Write the digits to the display.
    parent->ssd->writeDigitNum(0, minutes / 10);
    parent->ssd->writeDigitNum(1, minutes % 10);
    parent->ssd->writeDigitNum(3, seconds / 10);
    parent->ssd->writeDigitNum(4, seconds % 10);
    parent->ssd->writeColon();
    parent->ssd->writeDisplay();
  }

  if (parent->gameState->getGameState() == ModuleMode::Armed) {
    if (counter++ % 15 == 0) {
      // Increment the display counter.
      displayCounter++;

      // PRint the indicator information to the lcd display.
      if (parent->indicatorRows != 1 || parent->updateText) {
        // Determine the index and length of the display text.
        uint8_t index = displayCounter % parent->indicatorRows;
        uint8_t length = strlen(parent->indicatorText[index]);

        // Positition cursor to the indicator row and clears the row.
        parent->lcd->setCursor(0, 1);

        // Display the actual indicator content.
        parent->lcd->print(parent->indicatorText[index]);

        // Print whitespace over the rest of the line.
        for (uint8_t i = 0; i < 20 - length; i++) {
          parent->lcd->print(" ");
        }
      }

      // Print the port information to the lcd display.
      if (parent->portRows != 1 || parent->updateText) {
        // Determine the index and length of the display.
        uint8_t index = displayCounter % parent->portRows;
        uint8_t length = strlen(parent->portText[index]);

        // Positition cursor to the port row and clears the row.
        parent->lcd->setCursor(0, 2);

        // Display the actual indicator content.
        parent->lcd->print(parent->portText[displayCounter % parent->portRows]);

        // Print whitespace over the rest of the line.
        for (uint8_t i = 0; i < 20 - length; i++) {
          parent->lcd->print(" ");
        }

        parent->updateText = false;
      }
    }
  }

  return 100;
}
