#include <Arduino.h>
#include "ButtonManager.h"

ButtonManager::ButtonManager(unsigned int start, unsigned int n, ShiftIn* in) {
  this->start = start;
  this->n = n;
  this->in = in;
  this->buttons = new Button[n];
  for (int i = 0; i < n; i++) {
    buttons[i].setID(i + start);
  }
}

void ButtonManager::attachAllOnPress(ButtonListener *listener) {
  for (int i = 0; i < n; i++) {
    buttons[i].attachOnPress(listener);
  }
}

void ButtonManager::attachAllOnRelease(ButtonListener *listener) {
  for (int i = 0; i < n; i++) {
    buttons[i].attachOnRelease(listener);
  }
}

void ButtonManager::detachAllOnPress() {
  for (int i = 0; i < n; i++) {
    buttons[i].detachOnPress();
  }
}

void ButtonManager::detachAllOnRelease() {
  for (int i = 0; i < n; i++) {
    buttons[i].detachOnRelease();
  }
}

Button* ButtonManager::getButton(unsigned int n) {
  return buttons + n;
}

void ButtonManager::load() {
  for (int i = 0; i < n; i++) {
    buttons[i].load(in->getBit(start + i));
  }
}

ButtonManager::~ButtonManager() {
  delete [] buttons;
}

