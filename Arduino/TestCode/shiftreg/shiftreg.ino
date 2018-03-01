#define GREEN_PIN 2
#define DATA_PIN 3
#define RCLK_PIN 4
#define SRCLK_PIN 5

#include "ShiftReg.h"

byte count;
byte count1;
static unsigned int pins[] = {DATA_PIN, GREEN_PIN};
ShiftReg reg;


void setup() {
  //Serial.begin(9600);
  reg.build(1, 2, pins, RCLK_PIN, SRCLK_PIN);
  count = 0;
  count1 = 1;

}

void loop() {
  // put your main code here, to run repeatedly:
  reg.setReg(0, 0, count);
  reg.setReg(0, 1, count1);
  
  reg.load();
  count++;
  count1 = ((~count1) << 7) | (count1 >> 1);

  delay(250);
}
