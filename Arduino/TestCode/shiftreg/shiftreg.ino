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
#include <ButtonManager.h>
#include "myButtonListener.h"

static unsigned int pins[] = {BLUE_PIN, GREEN_PIN, RED_PIN};
ShiftOut out;
ShiftIn in(REG_CLOCK, LOAD_PIN, DATA_PIN, 1);
RGB_LED led(&out, 0, 16, 0, 8, 0, 0);
ButtonManager buttons(0, 8, &in);
MyButtonListener listener(&led, 2, 3);


void setup() {
  Serial.begin(9600);
  out.build(3, 1, pins, RCLK_PIN, SRCLK_PIN);
  buttons.attachAllOnPress(&listener);
  buttons.attachAllOnRelease(&listener);
}

void loop() {
  // put your main code here, to run repeatedly:
  out.load();
  in.load();
  buttons.load();
}
