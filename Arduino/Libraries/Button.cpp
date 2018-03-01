#include <Arduino.h>
#include "Button.h"

Button::Button(unsigned int n, ShiftIn *in) {
  this->n = n;
  this->in = in;
  value = in->getBit(n) ? ~0: 0;
  prs = nullptr;
  rel = nullptr;
}

unsigned int Button::getN() {
  return n;
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

void Button::detachonRelease() {
  this->rel = nullptr;
}

void Button::check() {
  value = value << 1;
  value |= in->getBit(n);
  if (value == 127 && prs != nullptr) {
    prs->onEvent(this, ButtonEvent::PRESS);
  } else if (value == 254 && rel != nullptr) {
    rel->onEvent(this, ButtonEvent::RELESE);
  }
}

Button::~Button() {}

