#ifndef SEG_7_H
#define SEG_7_H

/**
 * This is a seven segment display driver. Use it with the Sift Out
 * object. 
 * 
 * The folling chart shows the cofiguration of the display:
 *    
 *    __4
 * 5 |6 | 3
 *    --
 * 2 |__| 0
 *     1
 *     
 * The wiring must follow the following:
 * 
 * 6 5 G 4 3
 *   ______
 *  |  __  |
 *  | |  | |
 *  |  --  |
 *  | |__|*| 
 *  |______|
 * 2 1 X 0 X
 * 
 * If you devate form this you must reassign the segMap to match.
 */

#include <Arduino.h>
#include <ShiftOut.h>

// This is the map of what segments to activate for the display
extern byte segMap[];

class Seg7 final {
  public:

  /**
   * This is the constructor for the chain of seven seg displays
   * 
   * @param out the output object to be used
   * @param m the which chain of shift registors to use
   * @param n the nth bit where the chain begins
   */
  Seg7(ShiftOut *out, byte m, unsigned int n);

  /**
   * This sets a value on the display
   * 
   * @param n which display down the line to set
   * @param num the value to display
   */
  void setNum(unsigned int n, byte num);

  /**
   * This sets an ellement on the display
   * 
   * @param n which display to set
   * @param set the segement of the display to set
   * @param val the value of the segment
   */
  void setSeg(unsigned int n, byte seg, bool val);

  /**
   * This turns off a display
   * 
   * @param the display to be turned off
   */
  void mute(unsigned int n);

  private:
  ShiftOut *out;
  unsigned int n;
  byte m;

};


#endif // SEG_7_H
