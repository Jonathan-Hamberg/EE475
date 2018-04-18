#include "ShiftOut.h"
#include <Arduino.h>

ShiftOut::ShiftOut() {}

void ShiftOut::build(unsigned int reg_n, unsigned int m, const unsigned int *pins, unsigned int rclk, unsigned int srclk) {
  this->n = reg_n;
  this->m = m;
  this->rclk = rclk;
  this->srclk = srclk;

  pinMode(rclk, OUTPUT);
  pinMode(srclk, OUTPUT);

  this->pins = new unsigned byte[m];
  for (unsigned int i = 0; i < m; i++) {
    this->pins[i] = pins[i];
    pinMode(pins[i], OUTPUT);
  }

  this->values = new byte[m*n];
  this->setAll(false);
  this->load();
}

void ShiftOut::setAll(bool value) {
  hasChanged = true;
  byte val = value ? ~0 : 0;
  for (unsigned int i = 0; i < index(n, m); i++) {
    values[i] = val;
  }
}

void ShiftOut::set(unsigned int val_n, unsigned int m, bool value) {
  hasChanged = true;
  unsigned int ind = index((val_n >> 3), m);
  byte val = values[ind];
  byte mask = 1 << (val_n & 7);
  if (value) {
    val |= mask;
  } else {
    val &= ~mask;
  }
  values[ind] = val;
}

void ShiftOut::setReg(unsigned int reg_n, unsigned int m, byte value) {
  hasChanged = true;
  values[index(reg_n, m)] = value;
}

void ShiftOut::load() {
  if (!hasChanged) return;
  hasChanged = false;
  digitalWrite(srclk, LOW);
  digitalWrite(rclk, LOW);
  
  for (int i = n - 1; i >= 0; i--) {
    for (int b = 7; b >= 0; b--) {
      // I hate myself for this but it works
      for (volatile int j = 0; j < m; j++) {
        byte val = values[index(i, j)];
        bool set = extract(val, b);
        digitalWrite(pins[j], set);
      }
      digitalWrite(srclk, HIGH);
      digitalWrite(srclk, LOW);
    }
  }
  digitalWrite(rclk, HIGH);
  digitalWrite(rclk, LOW);
}

unsigned int ShiftOut::index(unsigned int reg_n, unsigned int m) {
  return m * (this->n) + reg_n;
}

bool ShiftOut::extract(byte b, int pos) {
  return (!!(b & (1<<pos)));
}

ShiftOut::~ShiftOut() {
  delete[] values;
  delete[] pins;
}

