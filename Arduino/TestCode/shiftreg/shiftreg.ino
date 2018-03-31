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
#include "GameState.h"
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
GameState game;


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

  Serial.println("-Building Game-");
  game.init(59, 3, 1);
  Serial.print("Game Time: ");
  Serial.println(game.getTimeLeft());
  Serial.print("Max Strikes: ");
  Serial.println(game.getMaxStrikes());
  Serial.print("Current Strikes: ");
  Serial.println(game.getStrikes());
  Serial.print("Current Game State: ");
  Serial.println(game.getGameState());
  Serial.print("SN : ");
  Serial.println(game.getSN());
  Serial.print("SN has vowel: ");
  Serial.println(game.SNhasVowel());
  Serial.print("SN is even: ");
  Serial.println(game.SNisEven());
  Serial.print("Batteries: ");
  Serial.println(game.getBatteries());
  Serial.println("-Indicator States-");
  Serial.print("SND: ");
  Serial.println(game.checkSND());
  Serial.print("CLR: ");
  Serial.println(game.checkCLR());
  Serial.print("CAR: ");
  Serial.println(game.checkCAR());
  Serial.print("IND: ");
  Serial.println(game.checkIND());
  Serial.print("FRQ: ");
  Serial.println(game.checkFRQ());
  Serial.print("SIG: ");
  Serial.println(game.checkSIG());
  Serial.print("NSA: ");
  Serial.println(game.checkNSA());
  Serial.print("MSA: ");
  Serial.println(game.checkMSA());
  Serial.print("TRN: ");
  Serial.println(game.checkTRN());
  Serial.print("BOB: ");
  Serial.println(game.checkBOB());
  Serial.print("FRK: ");
  Serial.println(game.checkFRK());
  Serial.println("-Port States-");
  Serial.print("DVI: ");
  Serial.println(game.checkDVI());
  Serial.print("Parallel: ");
  Serial.println(game.checkParallel());
  Serial.print("PS2: ");
  Serial.println(game.checkPS2());
  Serial.print("RJ45: ");
  Serial.println(game.checkRJ45());
  Serial.print("Serial: ");
  Serial.println(game.checkSerial());
  Serial.print("RCA: ");
  Serial.println(game.checkRCA());
  Serial.println("-Time Checks-");
  Serial.print("Time has 0: ");
  Serial.println(game.timeHasDigit(0));
  Serial.print("Time has 1: ");
  Serial.println(game.timeHasDigit(1));
  Serial.print("Time has 2: ");
  Serial.println(game.timeHasDigit(2));
  Serial.print("Time has 3: ");
  Serial.println(game.timeHasDigit(3));
  Serial.print("Time has 4: ");
  Serial.println(game.timeHasDigit(4));
  Serial.print("Time has 5: ");
  Serial.println(game.timeHasDigit(5));
  Serial.print("Time has 6: ");
  Serial.println(game.timeHasDigit(6));
  Serial.print("Time has 7: ");
  Serial.println(game.timeHasDigit(7));
  Serial.print("Time has 8: ");
  Serial.println(game.timeHasDigit(8));
  Serial.print("Time has 9: ");
  Serial.println(game.timeHasDigit(9));
}

void loop() {
  // put your main code here, to run repeatedly:
  out.load();
  in.load();
  buttons.load();
  t.load();
}
