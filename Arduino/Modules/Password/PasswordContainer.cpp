#include <Arduino.h>
#include "PasswordContainer.h"

#define NUM_PASSWORDS 35

const char phrases[][PASSWORD_LENGTH + 1] = {"ABOUT", "AFTER", "AGAIN", "BELOW", "COULD", "EVERY", "FIRST", "FOUND", "GREAT", "HOUSE", "LARGE", "LEARN", "NEVER", "OTHER", "PLACE", "PLANT", "POINT", "RIGHT", "SMALL", "SOUND", "SPELL", "STILL", "STUDY", "THEIR", "THERE", "THESE", "THING", "THINK", "THREE", "WATER", "WHERE", "WHICH", "WORLD", "WOULD", "WRITE"};

PasswordContainer::PasswordContainer() {
  
}

void PasswordContainer::build(lfsr *r) {
  uint8_t phraseIndex = r->next() % NUM_PASSWORDS;

  for (unsigned int i = 0; i < PASSWORD_LENGTH; i++ ) {
    text[i][0] = phrases[phraseIndex][i];
  }
  int count = 0;
  do {
    for (unsigned int i = 0; i < PASSWORD_LENGTH; i++) {
      for (unsigned int j = 1; j < PASSWORD_DEPTH; j++) {
        bool good = true;
        do {
          good = true;
          text[i][j] = 'A' + r->next() % 26;
          for (unsigned int k = 0; k < j; k++) {
            if (text[i][k] == text[i][j]) good = false;
            //Serial.print(count++);
            //Serial.print(", ");
            //Serial.println(text[i][j]);
            //if (k == j) Serial.println("BAD");
          }
        } while (!good);
      }
    }
  } while (numPossible() != 1);

  for (unsigned int i = 0; i < PASSWORD_LENGTH; i++) {
    index[i] = r->next() % PASSWORD_DEPTH;
  }
}

char PasswordContainer::getCurrentLetter(uint8_t n) {
  return text[n][index[n]];
}

void PasswordContainer::moveUp(uint8_t n) {
  index[n] = (index[n] + 1) % PASSWORD_DEPTH;
}

void PasswordContainer::moveDown(uint8_t n) {
  index[n] = (index[n] - 1) % PASSWORD_DEPTH;
}

bool PasswordContainer::isSolved() {
  for (unsigned int i = 0; i < PASSWORD_LENGTH; i++) {
    if (index[i] != 0) {
      return false;
    }
  }
  return true;
}

uint16_t PasswordContainer::numPossible() {
  uint8_t possible = 0;
  for (unsigned int i = 0; i < NUM_PASSWORDS; i++) {
    if (hasPassword(phrases[i])) {
      possible++;
    }
  }
  //Serial.print("possible ");
  //Serial.println(possible);
  return possible;
}

bool PasswordContainer::hasPassword(const char * password) {
  for (unsigned int i = 0; i < PASSWORD_LENGTH; i++) {
    bool found = false;
    for (unsigned int j = 0; j < PASSWORD_DEPTH; j++) {
      if (password[i] == text[i][j]) {
        found = true;
      }
    }
    if (!found) {
      return false;
    }
  }
  return true;
}

