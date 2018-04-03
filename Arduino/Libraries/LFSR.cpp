#ifdef __linux__
#include <stdlib.h>
#else
#include <Arduino.h>
#endif

#include "LFSR.h"

lfsr::lfsr(uint32_t seed, uint32_t poly) {
  while (seed == 0) {
    seed = rand();
  }
  this->seed = seed;
  this->poly = poly;
}


void lfsr::reseed(uint32_t seed) {
  this->seed = seed;
}

void lfsr::setPoly(uint32_t poly) {
  this->poly = poly;
}

uint32_t lfsr::next() {
  uint8_t lsb = seed & 1;
  seed >>= 1;
  if (lsb) seed ^= poly;
  return seed;
}

