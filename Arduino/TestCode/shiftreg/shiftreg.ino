#define GREEN_PIN 2
#define BLUE_PIN 3
#define RED_PIN 6
#define RCLK_PIN 4
#define SRCLK_PIN 5
#define REG_CLOCK 5
#define DATA_PIN 7
#define LOAD_PIN 8

#include <ShiftOut.h>
#include <ShiftIn.h>
#include <RGB_LED.h>
#include <Button.h>

static unsigned int pins[] = {BLUE_PIN, GREEN_PIN, RED_PIN};
ShiftOut out;
ShiftIn in(REG_CLOCK, LOAD_PIN, DATA_PIN, 1);
RGB_LED led(&out, 0, 16, 0, 8, 0, 0);
Button button(0, &in);


void setup() {
  //Serial.begin(9600);
  out.build(3, 1, pins, RCLK_PIN, SRCLK_PIN);
  pinMode(REG_CLOCK, OUTPUT);
  pinMode(DATA_PIN, INPUT);
  pinMode(LOAD_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte value = in.getReg(1);
  out.setReg(0,0,value);
  out.load();
  in.load();
}
