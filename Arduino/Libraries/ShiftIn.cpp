#include <Arduino.h>
#include "ShiftIn.h"

ShiftIn:: ShiftIn(byte clk_pin, byte load_pin, byte data_pin, byte regs) {
  this->clk_pin = clk_pin;
  this->load_pin = load_pin;
  this->data_pin = data_pin;
  this->regs = regs;
  this->data = new byte[regs];
  pinMode(clk_pin, OUTPUT);
  pinMode(load_pin, OUTPUT);
  pinMode(data_pin, INPUT);
  digitalWrite(load_pin, HIGH);
  digitalWrite(clk_pin, LOW);
  load();
}

bool ShiftIn::getBit(unsigned int n) {
  unsigned int index = n / 8;
  byte val = data[index];
  index = n % 8;
  return (val >> index) & 1;
}

byte ShiftIn::getReg(byte reg) {
  return data[reg];
}

void ShiftIn::load() {
  digitalWrite(clk_pin, LOW);
  digitalWrite(load_pin, HIGH);
  digitalWrite(load_pin, LOW);
  digitalWrite(load_pin, HIGH);

  for(int n = 0; n < regs; n++) {
    byte value = 0;
    for(volatile int i = 0; i < 8; i++) {
      value = value << 1;
      value |= digitalRead(data_pin);
      digitalWrite(clk_pin, HIGH);
      digitalWrite(clk_pin, LOW);
    }
    data[n] = value;
  }
}

ShiftIn::~ShiftIn() {
  delete[] data;
}



