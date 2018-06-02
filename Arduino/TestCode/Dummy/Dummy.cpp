#include "Dummy.h"
#include <Arduino.h>

Dummy::Dummy(GameState * /* game */)
    : buttonListener(this), timerListener(this) {
}

void Dummy::init(uint32_t /*seed*/) {
}

void Dummy::start() {
}

void Dummy::demo() {
}

void Dummy::explode() {
}

void Dummy::muteOutput() {
}

void Dummy::setMode(uint8_t /* mode */) {
  (void)(mode);
}

void Dummy::setColor(uint32_t /* color */) {
}

bool Dummy::quickPress() {
  return false;
}

Dummy::DummyButtonListener::DummyButtonListener(Dummy *parent) {
  this->parent = parent;
}

void Dummy::DummyButtonListener::onEvent(Button * /* caller */,
                                         ButtonEvent /* event */) {
}

Dummy::DummyTimerListener::DummyTimerListener(Dummy *parent) {
  this->parent = parent;
}

unsigned int Dummy::DummyTimerListener::onEvent(Timer * /* caller */,
                                                unsigned int /* id */,
                                                unsigned int /*calls*/) {
  return 0;

