#include <Arduino.h>
#include "Timer.h"

Timer::Timer(unsigned int n) {
  this->n = n;
  times = new timer[n];
  empty = times;
  start = nullptr;

  timer* current = empty;
  for (int i = 1; i<n; i++) {
    current->next = times + i;
    current = current->next;
    current->next = nullptr;
  }
}

Timer::timer * Timer::attachTimer(TimerListener* listener, unsigned int wait, unsigned int id) {
  unsigned long now = millis();
  if (empty == nullptr) return NULL;
  timer* current = empty;
  empty = empty->next;
  current->listener = listener;
  current->calls = 0;
  current->last = now;
  current->wait = wait;
  current->id = id;
  schedule(current);
  return current;
}

void Timer::detachTimer(timer * t) {
  if (t == nullptr) return;
  if (start == nullptr) return;

  if (start == t) {
    start = start->next;
    t->next = empty;
    empty = t;
    return;
  }

  timer * last = start;
  timer * current = start->next;

  while (current != nullptr) {
    if (current == t) {
      last->next = current->next;
      current->next = empty;
      empty = current;
      return;
    }
    last = current;
    current = current->next;
  }

  return;
}

void Timer::load() {
  timer *t = start;
  timer *stack = nullptr;

  // Building a stack of read to exicute timers
  unsigned long now = millis();
  unsigned long displacement = now - t->last;
  if (displacement >> 31) {
    displacement = t->last - now;
  }
  while (t != nullptr && 
        (displacement >= t->wait)) {
    start = t->next;
    t->next = stack;
    stack = t;
    t = start;
    now = millis();
    displacement = now - t->last;
    if (displacement >> 31) {
      displacement = t->last - now;
    }
  }

  // Exicuting every timer in the stack
  while (stack != nullptr) {
    unsigned int response = stack->listener->onEvent(this, stack->id, stack->calls);
    if (response == TIMER_STOP) {
      t = stack;
      stack = stack->next;
      t->next = empty;
      empty = t;
    } else {
      stack->calls += 1;
      stack->last += stack->wait;
      stack->wait = response;
      t = stack;
      stack = stack->next;
      schedule(t);
    }
  }
}

void Timer::schedule(timer *t) {
  timer *last = nullptr;
  timer *current = start;

  while (current != nullptr
    && ((current->last + current->wait) - t->last < t->wait)) {
      last = current;
      current = current->next;
   }

   if (last == nullptr) {
    start = t;
    t->next = current;
   } else {
    last->next = t;
    t->next = current;
   }
}

Timer::~Timer() {
  delete[] times;
}

