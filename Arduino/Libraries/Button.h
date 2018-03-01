#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <ShiftIn.h>

/**
 * The button class handles the operation of one button. This includes debounceing,
 * state holding, and both press and relese event handling.
 * 
 * Use the ButtonListener class as a parent class to create listener events.
 * A single listener may be used by multiple buttons and events. To allow the listener
 * to know how it was called it is passed both a pointer the the caller button and an event
 * value.
 */

class Button;

/**
 * This enum holds the diffent states of a button
 */
enum ButtonEvent {
  PRESS, RELESE, DOWN, UP
};

/**
 * This is the base button listener class. 
 * Create child classes to make new listener events
 */
class ButtonListener {
  public:

  /**
   * This the onEvent function defintion. This is function is called
   * whenever its event happens.
   * 
   * @param caller a pointer to the caller button
   * @param event the event that happended
   */
  virtual void onEvent(Button* caller, ButtonEvent event);
};

/**
 * This the button class
 */
class Button final {
  public:

  /**
   * This is the button constructor.
   * 
   * @param n the n-th bit down the registor for the button
   * @param in the shiftin object that holds the value for the buttons raw input
   */
  Button(unsigned int id);

  /**
   * default constructor assigns id to 0
   */
  Button();

  /**
   * @return the location of the button on the shift registor
   */
  unsigned int getID();

  /**
   * Use this funciton to set the button id
   */
  void setID(unsigned int id);

  /**
   * @return the current state of the button
   */
  bool getValue();

  /**
   * attaches a on press handler
   * 
   * @param prs the on press event handler
   */
  void attachOnPress(ButtonListener* prs);
  void attachOnRelease(ButtonListener* rel);

  void detachOnPress();
  void detachOnRelease();

  void load(bool val);

  ~Button();

  private:
  unsigned int id;
  byte value;
  ButtonListener *prs, *rel;
};

#endif // BUTTON_H
