#ifndef LFSR_H
#define LFSR_H

#include <stdint.h>
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
  lfsr(uint32_t seed = 0, uint32_t poly = (1UL << 31) | (1UL << 21) | (1UL << 1) | 1);

  /**
   * This changes the seed of the lfsr
   * 
   * @param seed the new seed
   */
  void reseed(uint32_t seed);

  /**
   * This changes the polynomial
   * 
   * @param poly the new polynomial
   */
  void setPoly(uint32_t poly);

  /**
   * Returns the next value in the lfsr
   */
  uint32_t next();
  
  private:

  uint32_t seed;
  uint32_t poly;
  
};

#endif // LFSR_H
