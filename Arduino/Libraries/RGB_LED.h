#ifndef RGB_LED_H
#define RGB_LED_H

#include <Arduino.h>
#include "ShiftOut.h"

/**
 * This is the an RGB LED object. It uses the ShiftReg object to set the value of a rgb led.
 * To do this, the object uses the base location of the red, green, and blue channel of the first
 * led in seires. For this to work, all the addressed leds must be consecutivly after the addressed rgb led.
 * LED channels may be on diffrent registor lines as long as the red, greens, and blues are grouped together.
 */

/*
 * These defines can be used to set color with from the lower 3 bits of a value.
 * Color inputs take the form xxxxxRGB in binary.
 */
#define LED_BLACK 0
#define LED_BLUE 1
#define LED_GREEN 2
#define LED_CYAN 3
#define LED_RED 4
#define LED_PURPLE 5
#define LED_YELLOW 6
#define LED_WHITE 7

typedef byte led_color;

class RGB_LED final {
  public:

  /**
   * This is the rgb led constructor. The m and n values corispond to the
   * m and n locations of the first led in each color.
   * 
   * n values are always by index not by registor
   * 
   * @param reg the ShiftReg object to use
   * @param rm the red m value
   * @param rn the red n value
   * @param gm the green m value
   * @param gn the green n value
   * @param bm the blue m value
   * @param bn the blue n value
   */
  RGB_LED(ShiftOut* reg, unsigned int rn, unsigned int rm, unsigned int gn, unsigned int gm, unsigned int bn, unsigned int bm);

  /**
   * This function sets the color of an LED.
   * 
   * @param led the index of the led relitive to the first led
   * @param color the color to set the led
   */
  void set(unsigned int led, led_color color);

  /**
   * destuctor
   */
  virtual ~RGB_LED();
  
  private:
  ShiftOut* reg;
  unsigned int rm, rn, gm, gn, bm, bn;
};

#endif // RGB_LED_H
