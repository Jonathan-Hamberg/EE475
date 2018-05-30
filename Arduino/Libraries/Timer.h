#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

/**
 * This is a timer class. It calls a timer listener at a specified number of
 * milliseconds in the future.
 *
 * The timer can handle a upto a specified number of timers at once.
 */

#define TIMER_STOP 0

class Timer;

/**
 * This is the base class for a timer listener. Use this class
 * as a parent class to create listeners.
 */
class TimerListener {
 public:
  /**
   * The on event function is called when ever a timers time is up.
   * This function will return the time to the next call or zero to stop all
   * calls.
   *
   * @param caller a pointer to the caller object
   * @param id the id of the timer
   * @param calls the number of times the timer has been called
   */
  virtual unsigned int onEvent(Timer *caller, unsigned int id,
                               unsigned int calls);

 private:
};

class Timer final {
 public:
  struct timer {
    TimerListener *listener;
    unsigned int id;
    unsigned int calls;
    unsigned long last;
    unsigned long wait;
    timer *next;
  };

  /**
   * This is the constructor for the timer object.
   *
   * @param n the max number of timers that can be active
   */
  Timer(unsigned int n);

  /**
   * This function attaches a listener to a new timer
   *
   * @param listener a pointer to the listener
   * @param wait how long to wait before calling the listener
   * @param id an id for the timer defaults to 0
   * @return address of the timer
   */
  timer *attachTimer(TimerListener *listener, unsigned int wait,
                     unsigned int id = 0);

  void detachTimer(timer *t);

  void load();

  ~Timer();

 private:
  unsigned int n;
  timer *times, *start, *empty;

  void schedule(timer *t);
};

#endif  // TIMER_H
