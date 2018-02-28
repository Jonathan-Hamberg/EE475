#include "ShiftReg.h"
#include <Arduino.h>

ShiftReg::ShiftReg() {}

void ShiftReg::build(unsigned int reg_n, unsigned int m, unsigned int *pins, unsigned int rclk, unsigned int srclk) {
  this->n = reg_n;
  this->m = m;
  this->rclk = rclk;
  this->srclk = srclk;

  pinMode(rclk, OUTPUT);
  pinMode(srclk, OUTPUT);

  this->pins = new unsigned byte[m];
  for (int i = 0; i < m; i++) {
    this->pins[i] = pins[i];
    pinMode(pins[i], OUTPUT);
  }

  this->values = new byte[m*n];
  this->setAll(false);
  this->load();
}

void ShiftReg::setAll(bool value) {
  byte val = value ? ~0 : 0;
  for (int i = 0; i < index(n, m); i++) {
    values[i] = val;
  }
}

void ShiftReg::set(unsigned int val_n, unsigned int m, bool value) {
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

void ShiftReg::setReg(unsigned int reg_n, unsigned int m, byte value) {
  values[index(reg_n, m)] = value;
}

void ShiftReg::load() {
  digitalWrite(srclk, LOW);
  digitalWrite(rclk, LOW);
  
  for (int i = n - 1; i >= 0; i--) {
    for (int b = 7; b >= 0; b--) {
      for (int j = 0; j < m; j++) {
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

unsigned int ShiftReg::index(unsigned int reg_n, unsigned int m) {
  return m * (this->n) + reg_n;
}

bool ShiftReg::extract(byte b, int pos) {
  return (!!(b & (1<<pos)));
}

ShiftReg::~ShiftReg() {
  delete values;
  delete pins;
}

