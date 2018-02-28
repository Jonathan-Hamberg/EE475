#ifndef SHIFT_REG_H
#define SHIFT_REG_H

#include <Arduino.h>

/**
 * This is a simple library for running multiple shift registors.
 * The library can run n by m set of registors using the same control clocks for each registor.
 *
 * To create a ShiftReg object call the build function after using the default constructor.
 * 
 * @authors Daniel Predmore, Christy Truong, Jonathan Hamberg
 */


class ShiftReg 
{
  public:

  /**
   * This is the default constructor. The build function must be called before using any other funciton
   * in the class.
   */
  ShiftReg();

  /**
   * This function sets up the ShiftReg class.
   * 
   * The build function should be called once and only once. The pins argument should have a lenght of n and
   * does not need to exist after the function call.
   * 
   * To use the object with only one clock, set rclk = srclk
   * 
   * @param reg_n the number of registors in seires
   * @param m the number of parallel registors in the system
   * @param pins the data pin number for each output
   * @param rclk the pin number of the rclk
   * @param srclk the pin number of the srclk
   */
  void build(unsigned int reg_n, unsigned int m, const unsigned int *pins, unsigned int rclk, unsigned int srclk);

  /**
   * This funciton sets all the values in the object to either high or low
   * 
   * @param value the value to set all of the values to
   */
  void setAll(bool value);

  /**
   * Use this function to set a value of a single bit on a shift registor
   * 
   * @param val_n which bit down the chain to set
   * @param m which parallel set registors to set
   * @param value the value to set the bit
   */
  void set(unsigned int val_n, unsigned int m, bool value);

  /**
   * This function sets all the values of a registor at once.
   * 
   * @param reg_n the number of the registors down the line to set
   * @param m which paralles set or registors to set
   * @param value each bit of the value corisponds to that bit of the registor
   */
  void setReg(unsigned int reg_n, unsigned int m, byte value);

  /**
   * Call this function to update the registores
   */
  void load();

  /**
   * This is the destructor
   */
  virtual ~ShiftReg();

  private:

  inline unsigned int index(unsigned int reg_n, unsigned int m);
  inline bool extract(byte b, int pos);
  
  byte m, n, rclk, srclk;
  byte * pins;
  byte * values;
};

#endif // SHIFT_REG_H

