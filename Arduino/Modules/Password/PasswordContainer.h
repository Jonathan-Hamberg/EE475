#ifndef PASSWORD_CONTAINER_H
#define PASSWORD_CONTAINER_H

#include <Arduino.h>
#include <LFSR.h>

#define PASSWORD_LENGTH 5
#define PASSWORD_DEPTH 5

class PasswordContainer final {
  public:

  PasswordContainer();

  void build(lfsr *r);

  char getCurrentLetter(uint8_t n);

  void moveUp(uint8_t n);

  void moveDown(uint8_t n);

  bool isSolved();
  
  private:

  uint16_t numPossible();

  bool hasPassword(char * password);

  uint8_t index[PASSWORD_LENGTH];
  char text[PASSWORD_LENGTH][PASSWORD_DEPTH];
};

#endif // PASSWORD_CONTAINER_H
