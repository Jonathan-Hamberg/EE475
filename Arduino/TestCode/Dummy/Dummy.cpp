#include <Arduino.h>
#include "Dummy.h"

Dummy::Dummy(GameState * game) : buttonListener(this), timerListener(this) {
    (void)(game);
}

void Dummy::init(uint32_t seed) {
    // Unused parameter warning.
    (void)(seed);
}

void Dummy::start() {
}

void Dummy::demo() {
}

void Dummy::explode() {
}

Dummy::~Dummy() {
}

void Dummy::muteOutput() {
}

void Dummy::setMode(uint8_t mode) {
    (void)(mode);
}

void Dummy::setColor(uint32_t color) {
    (void)(color);

}

bool Dummy::quickPress() {
  return false;
}

Dummy::DummyButtonListener::DummyButtonListener(Dummy * parent) {
    // Unused parameter warning.
    (void)(parent);
}

void Dummy::DummyButtonListener::onEvent(Button * caller, ButtonEvent event) {
    // Unused parameter warning.
    (void)(caller);
    (void)(event);
}

Dummy::DummyTimerListener::DummyTimerListener(Dummy * parent) {
    // Unused paramter warning.
    (void)(parent);
}

unsigned int Dummy::DummyTimerListener::onEvent(Timer *caller, unsigned int id, unsigned int calls) {
    // Unused parameter warning.
    (void)(caller);
    (void)(id);
    (void)(calls);
  return 0;
}

