#include <Arduino.h>
#include "RGB_LED.h"

RGB_LED::RGB_LED(ShiftOut* reg, unsigned int rn, unsigned int rm, unsigned int gn, unsigned int gm, unsigned int bn, unsigned int bm) {
  this->reg = reg;
  this->rm = rm;
  this->rn = rn;
  this->gm = gm;
  this->gn = gn;
  this->bm = bm;
  this->bn = bn;
}

void RGB_LED::set(unsigned int led, led_color color) {
  bool blue = color & 1;
  bool green = (color >> 1) & 1;
  bool red = (color >> 2) & 1;
  reg->set(bn + led, bm, blue);
  reg->set(gn + led, gm, green);
  reg->set(rn + led, rm, red); 
}

RGB_LED::~RGB_LED() {}

