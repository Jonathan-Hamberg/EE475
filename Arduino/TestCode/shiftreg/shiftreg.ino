#define GREEN_PIN 2
#define BLUE_PIN 3
#define RED_PIN 6
#define RCLK_PIN 4
#define SRCLK_PIN 5
#define REG_CLOCK 5
#define DATA_PIN 7
#define LOAD_PIN 8
#define RS 2
#define EN 6
#define D7 12
#define D6 11
#define D5 10
#define D4 9

#include <ShiftOut.h>
#include <ShiftIn.h>
#include <RGB_LED.h>
#include <Button.h>
#include <ButtonManager.h>
#include "LFSR.h"
#include "Seg7.h"
#include "Timer.h"
#include "myButtonListener.h"
#include "myTimerListener.h"
#include <LiquidCrystal.h>


static unsigned int pins[] = {BLUE_PIN, GREEN_PIN, RED_PIN};
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
ShiftOut out;
ShiftIn in(REG_CLOCK, LOAD_PIN, DATA_PIN, 1);
RGB_LED led(&out, 16, 0, 8, 0, 0, 0);
ButtonManager buttons(0, 8, &in);
Seg7 seg(&out, 0, 24);
MyButtonListener listener(&led, &seg, &out, &lcd, 2, 3);
lfsr r;
MyTimerListener l(&led, &r, &lcd, 0);
Timer t(8);


void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  lcd.begin(16, 2);
  out.build(6, 1, pins, RCLK_PIN, SRCLK_PIN);
  buttons.attachAllOnPress(&listener);
  buttons.attachAllOnRelease(&listener);
  t.attachTimer(&l, 0, 7);
  t.attachTimer(&l, 75, 6);
  t.attachTimer(&l, 150, 5);
  t.attachTimer(&l, 225, 4);
  t.attachTimer(&l, 300, 3);
  t.attachTimer(&l, 375, 2);
  t.attachTimer(&l, 450, 1);
  t.attachTimer(&l, 525, 0);
  seg.mute(0);
  seg.mute(1);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  out.load();
  in.load();
  buttons.load();
  t.load();
}
