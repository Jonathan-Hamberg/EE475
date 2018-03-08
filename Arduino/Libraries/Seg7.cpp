#include <Arduino.h>
#include "Seg7.h"

byte segMap[] = {64, ~9,(byte) ~94, 36, (byte) ~105, 12, 8, (byte) ~25, 0, 4};

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



