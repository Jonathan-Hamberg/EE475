#include "Control.h"
#include <Arduino.h>

Control::Control(ShiftIn* in, ShiftOut* out, RGB_LED* led,
                 ButtonManager* buttons, Timer* t,
                 ArduinoGameManager* gameManager, LiquidCrystal_I2C* lcd,
                 Adafruit_7segment* ssd)
    : buttonListener(this), timerListener(this) {
  this->in = in;
  this->out = out;
  this->led = led;
  this->buttons = buttons;
  this->t = t;
  this->lcd = lcd;
  this->ssd = ssd;
  this->currentTimer = nullptr;
  this->gameState = &gameManager->getGameState();
  this->gameManager->setModuleType(ModuleType::Control);

  buttons->attachAllOnPress(&buttonListener);
  buttons->attachAllOnRelease(&buttonListener);
}

void Control::init(uint32_t seed) {
  (void)(seed);

  muteOutput();
  setMode(ModuleMode::Off);

  // Initialize the timer object.
  this->t->detachTimer(currentTimer);
  currentTimer = this->t->attachTimer(&timerListener, 0);

  // Reset each of the lines to be empty by setting the first character to be
  // null.
  for (uint8_t i = 0; i < 3; i++) { indicatorText[i][0] = '\0'; }

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
  for (uint8_t i = 0; i < 2; i++) { portText[i][0] = '\0'; }
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

  // Update the serial number on the
  lcd->setCursor(0, 3);
  lcd->print("\n");
  lcd->setCursor(0, 3);
  lcd->print(gameState->getSN());

  muteOutput();
  setMode(ModuleMode::Off);
}

void Control::start() {
  muteOutput();
  setMode(ModuleMode::Armed);
  lcd->setCursor(0, 0);
}

void Control::demo() {
  muteOutput();
  setMode(ModuleMode::Demo);
  lcd->setCursor(0, 0);
}

void Control::explode() {
  muteOutput();
  setMode(ModuleMode::Off);
  lcd->setCursor(0, 0);
}

Control::~Control() {
}

void Control::setMode(ModuleMode mode) {
  // Select the first line of the LCD.
  /* lcd->setCursor(0, 0); */
  this->mode = mode;
  switch (mode) {
    case ModuleMode::Off:
      lcd->print("Off                 ");
      break;
    case ModuleMode::Armed:
      led->set(0, LED_RED);
      lcd->print("Armed               ");
      break;
    case ModuleMode::Disarmed:
      led->set(0, LED_GREEN);
      lcd->print("Disarmed            ");
      break;
    case ModuleMode::Demo:
      led->set(0, LED_BLUE);
      lcd->print("Demo                ");
      break;
    default:
      break;
  }
  return;
}

void Control::muteOutput() {
  led->set(0, LED_BLACK);
  out->set(RED_INDEX, 0, false);
  out->set(BLUE_INDEX, 0, false);
  out->set(GREEN_INDEX, 0, false);
  out->set(YELLOW_INDEX, 0, false);
}

Control::ControlButtonListener::ControlButtonListener(Control* parent) {
  this->parent = parent;
}

void Control::ControlButtonListener::onEvent(Button* caller,
                                             ButtonEvent event) {
  (void)(caller);
  (void)(event);
}

Control::ControlTimerListener::ControlTimerListener(Control* parent) {
  this->parent = parent;
}

unsigned int Control::ControlTimerListener::onEvent(Timer* caller,
                                                    unsigned int id,
                                                    unsigned int calls) {
  (void)(caller);
  (void)(id);
  (void)(calls);

  static uint32_t counter = 0;
  static uint32_t displayCounter = 0;

  if (counter++ % 15 == 0) {
    // Increment the display counter.
    displayCounter++;

    // Positition cursor to the indicator row and clears the row.
    parent->lcd->setCursor(0, 1);
    parent->lcd->print("                    ");
    parent->lcd->setCursor(0, 1);
    // Display the actual indicator content.
    parent->lcd->print(
        parent->indicatorText[displayCounter % parent->indicatorRows]);

    // Positition cursor to the port row and clears the row.
    parent->lcd->setCursor(0, 2);
    parent->lcd->print("                    ");
    parent->lcd->setCursor(0, 2);
    // Display the actual indicator content.
    parent->lcd->print(parent->portText[displayCounter % parent->portRows]);
  }

  return 100;
}
