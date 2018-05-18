#include <Arduino.h>
#include "TheButton.h"

#define STRING_LENGTH 17
#define DELAY 35

//                                          0123456789012345    0123456789012345    0123456789012345    0123456789012345
const static char text[][STRING_LENGTH] = {"     ABORT      ", "    DETONATE    ", "      HOLD      ", "     PRESS      "};
const static uint32_t colors[] = {NEO_RED, NEO_GREEN, NEO_BLUE, NEO_YELLOW, NEO_WHITE};

TheButton::TheButton(RGB_LED * led, Button * button, Timer * t, GameState * game,
       SPIManager* spiManager, LiquidCrystal_I2C * lcd, Adafruit_NeoPixel * strip): buttonListener(this), timerListener(this) {
  this->led = led;
  this->button = button;
  this->t = t;
  this->game = game;
  this->lcd = lcd;
  this->strip = strip;
  this->displayTxt[0] = '\0';
  this->displayTxt[17] = '\n';
  this->spiManager = spiManager;

  this->button->attachOnPress(&buttonListener);
  this->button->attachOnRelease(&buttonListener);

  this->currentTimer = nullptr;

  //muteOutput();
  setMode(ModuleMode::Off);
}

void TheButton::init(uint32_t seed) {
  r.reseed(seed);
  this->buttonColor = colors[r.next() % 5];
  this->downColor = colors[r.next() % 5];
  textIndex = r.next() % 4;
  for (int i = 0; i < 16; i++) {
    this->displayTxt[i] = text[textIndex][i];
  }

  muteOutput();
  setMode(ModuleMode::Off);
}

void TheButton::start() {
  muteOutput();
  setColor(buttonColor);
  lcd->print(displayTxt);
  setMode(ModuleMode::Armed);
  t->detachTimer(currentTimer);
  currentTimer = nullptr;
}

void TheButton::demo() {
  muteOutput();
  setMode(ModuleMode::Demo);
  t->detachTimer(currentTimer);
  currentTimer = t->attachTimer(&timerListener, 0, 0);
}

void TheButton::explode() {
  muteOutput();
  setMode(ModuleMode::Off);
  t->detachTimer(currentTimer);
  currentTimer = nullptr;
}

TheButton::~TheButton() {
  t->detachTimer(currentTimer);
  button->detachOnPress();
  button->detachOnRelease();
}

void TheButton::muteOutput() {
  lcd->clear();
  setColor(0);
  led->set(0, LED_BLACK);
}

void TheButton::setMode(ModuleMode mode) {
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
    case ModuleMode::Off:
      led->set(0, LED_BLACK);
    break;
    default: break;
  }
}

void TheButton::setColor(uint32_t color) {
  for (int i = 0; i < 16; i++) {
    strip->setPixelColor(i, color);
  }
  strip->show();
}

bool TheButton::quickPress() {
  if (buttonColor == NEO_BLUE && textIndex == 0) return false;
  else if (game->getBatteries() > 1 && textIndex == 1) return true;
  else if (buttonColor == NEO_WHITE && game->checkCAR()) return false;
  else if (game->getBatteries() > 2 && game->checkFRK()) return true;
  else if (buttonColor == NEO_YELLOW) return false;
  else if (buttonColor == NEO_RED && textIndex == 2) return true;
  else return false;
}

TheButton::TheButtonButtonListener::TheButtonButtonListener(TheButton * parent) {
  this->parent = parent;
}

void TheButton::TheButtonButtonListener::onEvent(Button * caller, ButtonEvent event) {
  Serial.println("Hello");
  if(parent->mode != ModuleMode::Armed) return;

  if (event == ButtonEvent::PRESS) {
    if(parent->currentTimer == nullptr) {
      parent->currentTimer = parent->t->attachTimer(&(parent->timerListener), DELAY, 0);
    }
    parent->setColor(0);
  } else {
    parent->t->detachTimer(parent->currentTimer);
    parent->currentTimer = nullptr;

    if (parent->quickPress()) {
        parent->spiManager->disarmModule();
      if (parent->currentTimer != nullptr) {
        parent->setMode(ModuleMode::Disarmed);
        parent->setColor(0);
      } else {
        parent->spiManager->strikeModule();
        parent->setColor(parent->buttonColor);
      }
    } else {
      switch (parent->downColor) {
        case NEO_BLUE:
          if (parent->game->timeHasDigit(4)) {
            parent->spiManager->disarmModule();
            parent->setMode(ModuleMode::Disarmed);
            parent->setColor(0);
          } else {
            parent->setColor(parent->buttonColor);
            parent->spiManager->strikeModule();
          }
        break;
        case NEO_WHITE:
          if (parent->game->timeHasDigit(1)) {

            parent->spiManager->disarmModule();
            parent->setMode(ModuleMode::Disarmed);
            parent->setColor(0);
          } else {
            parent->setColor(parent->buttonColor);
            parent->spiManager->strikeModule();
          }
        break;
        case NEO_YELLOW:
          if (parent->game->timeHasDigit(5)) {

            parent->spiManager->disarmModule();
            parent->setMode(ModuleMode::Disarmed);
            parent->setColor(0);
          } else {
            parent->setColor(parent->buttonColor);
            parent->spiManager->strikeModule();
          }
        break;
        default:
          if (parent->game->timeHasDigit(1)) {

            parent->spiManager->disarmModule();
            parent->setMode(ModuleMode::Disarmed);
            parent->setColor(0);
          } else {
            parent->setColor(parent->buttonColor);
            parent->spiManager->strikeModule();
          }
        break;
      }
    }
  }
}

TheButton::TheButtonTimerListener::TheButtonTimerListener(TheButton * parent) {
  this->parent = parent;
}

unsigned int TheButton::TheButtonTimerListener::onEvent(Timer *caller, unsigned int id, unsigned int calls) {
  if (calls == 16) {
    if (parent->quickPress()) {
      //send strike signal
      parent->setColor(parent->buttonColor);
    }
    parent->currentTimer = nullptr;
    return 0;
  }
  parent->strip->setPixelColor(calls, parent->downColor);
  parent->strip->show();
  return DELAY;
}

