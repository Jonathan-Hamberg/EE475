#ifndef LFSR_H
#define LFSR_H

#include <Arduino.h>

/**
 * This is a simple lfsr object
 */

class lfsr final {
  public:

  /**
   * This is the lfsr constructor
   * 
   * @param seed the seed value 
   * @param poly the polynomial to use
   */
  lfsr(unsigned long seed = 0, unsigned long poly = (((unsigned long ) 1)<< 31) | (((unsigned long ) 1) << 21) | (((unsigned long ) 1) << 1) | 1);

  /**
   * This changes the seed of the lfsr
   * 
   * @param seed the new seed
   */
  void reseed(unsigned long seed);

  /**
   * This changes the polynomial
   * 
   * @param poly the new polynomial
   */
  void setPoly(unsigned long poly);

  /**
   * Returns the next value in the lfsr
   */
  unsigned long next();
  
  private:

  unsigned long seed;
  unsigned long poly;
  
};

#endif // LFSR_H
