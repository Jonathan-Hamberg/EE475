#include <Arduino.h>
#include "LFSR.h"

lfsr::lfsr(unsigned long seed, unsigned long poly) {
  while (seed == 0) {
    seed = rand();
  }
  this->seed = seed;
  this->poly = poly;
}


void lfsr::reseed(unsigned long seed) {
  this->seed = seed;
}

void lfsr::setPoly(unsigned long poly) {
  this->poly = poly;
}

unsigned long lfsr::next() {
  byte lsb = seed & 1;
  seed >>= 1;
  if (lsb) seed ^= poly;
  return seed;
}

