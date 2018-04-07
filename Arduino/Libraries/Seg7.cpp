#include <Arduino.h>
#include "Seg7.h"

byte segMap[] = {uint8_t(64), uint8_t(~9),uint8_t(~94), uint8_t(36),
    uint8_t(~105), uint8_t(12), uint8_t(8), uint8_t(~25),
    uint8_t(0), uint8_t(4)};

Seg7::Seg7(ShiftOut *out, byte m, unsigned int n) {
  this->out = out;
  this->n = n;
  this->m = m;
}

void Seg7::setNum(unsigned int n, byte num) {
  byte val = segMap[num];
  for (int i = 0; i < 7; i++) {
    out->set(this->n + n * 7 + i, m, val & 1);
    val = val >> 1;
  }
}

void Seg7::setSeg(unsigned int n, byte seg, bool val) {
  out->set(this->n + n * 7 + seg, m, val);
}

void Seg7::mute(unsigned int n) {
  for (int i = 0; i < 7; i++) {
    out->set(this->n + n * 7 + i, m, 1);
  }
}



