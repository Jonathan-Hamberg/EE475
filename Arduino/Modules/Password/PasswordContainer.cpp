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

  do {
    for (unsigned int i = 0; i < PASSWORD_LENGTH; i++) {
      for (unsigned int j = 1; i < PASSWORD_DEPTH; i++) {
        text[i][j] = 'A' + r->next() % 26;
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
  return possible;
}

bool PasswordContainer::hasPassword(char * password) {
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

