#include <Arduino.h>
#include "Button.h"

Button::Button(unsigned int id) {
  this->id = id;
  value = 0;
  prs = nullptr;
  rel = nullptr;
}

Button::Button() {
  this->id = 0;
  prs = nullptr;
  rel = nullptr;
}

unsigned int Button::getID() {
  return id;
}

void Button::setID(unsigned int id) {
  this->id = id;
}

bool Button::getValue() {
  return (value & 127) == 127; 
}

void Button::attachOnPress(ButtonListener* prs) {
  this->prs = prs;
}

void Button::attachOnRelease(ButtonListener* rel) {
  this->rel = rel;
}

void Button::detachOnPress() {
  this->prs = nullptr;
}

void Button::detachOnRelease() {
  this->rel = nullptr;
}

void Button::load(bool val) {
  value = value << 1;
  value |= val;
  if (value == 127 && prs != nullptr) {
    prs->onEvent(this, ButtonEvent::PRESS);
  } else if (value == 254 && rel != nullptr) {
    rel->onEvent(this, ButtonEvent::RELEASE);
  }
}

Button::~Button() {}

