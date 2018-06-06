#include <LFSR.h>
#include "PasswordContainer.h"

lfsr r;
PasswordContainer c;

unsigned long max;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  r.reseed(analogRead(0));
  c.build(&r);
  for (unsigned int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      Serial.print(c.getCurrentLetter(i));
      Serial.print(", ");
      c.moveUp(i);
    }
    Serial.println();
  }
  max = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  //r.reseed(analogRead(0));
  unsigned long t = micros();
  c.build(&r);
  /*for (unsigned int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      Serial.print(c.getCurrentLetter(i));
      Serial.print(", ");
      c.moveUp(i);
    }
    Serial.println();
  }*/
  unsigned long runTime = micros() - t;
  if (runTime > max) {
    max = runTime;
    Serial.println(max);
  }
}
