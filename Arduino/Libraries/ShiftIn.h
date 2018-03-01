#ifndef SHIFT_IN_H
#define SHIFT_IN_H

#include <Arduino.h>

/**
 * This is the input shift registor object. It can read form a chain of 8 bit input
 * registors.
 */

class ShiftIn final {
  public:

  /**
   * This is the constructor for the ShiftIn object.
   * 
   * @param clk_pin the pin number of the registors clock
   * @param load_pin the pin number of the registors load pin
   * @param data_pin the pin number of the data pin
   * @param regs the number of registors to read
   */
  ShiftIn(byte clk_pin, byte load_pin, byte data_pin, byte regs);

  /**
   * This function fetches a bit form the last read of the registors
   * 
   * @param n the n-th bit to read
   */
  bool getBit(unsigned int n);

  /**
   * This function reads in all the values of a registor from the last read.
   * 
   * @param reg which registor to read
   */
  byte getReg(byte reg);

  /**
   * This updates the values from the registors
   */
  void load();

  /**
   * Destructor
   */
  ~ShiftIn();
  private:
  byte clk_pin, load_pin, data_pin, regs;
  byte* data;
};

#endif // SHIFT_IN_H
