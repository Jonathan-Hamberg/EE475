
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
  #undef __FlashStringHelper::F(string_literal)
  #define F(string_literal) string_literal
#endif

#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BACKGROUND 0x0680
#define BORDER 0x0019
#define TEXT 0XFFFF

void drawLayOut(Adafruit_TFTLCD *tft) {
  tft->fillScreen(BLACK);
  tft->fillRoundRect(0, 0, 240, 80, 10, BACKGROUND);
  tft->drawRoundRect(0, 0, 240, 80, 10, BORDER);

  for (unsigned int i = 0; i < 3; i++) {
    for (unsigned int j = 0; j < 2; j++) {
      tft->fillRoundRect(0 + j * 120, 80 + i * 80, 120, 80, 10, BACKGROUND);
      tft->drawRoundRect(0 + j * 120, 80 + i * 80, 120, 80, 10, BORDER);
    }
  }
}

void clearLayout(Adafruit_TFTLCD *tft) {
  tft->fillRect(20, 30, 200, 24, BACKGROUND);

  for (unsigned int i = 0; i < 3; i++ ) {
    for (unsigned int j = 0 ; j < 2; j++) {
      tft->fillRect(10 + j * 120, 110 + 80 * i, 100, 16, BACKGROUND);
    }
  }
}

void printTitle(Adafruit_TFTLCD *tft, const char * str) {
  tft->setTextColor(TEXT, BACKGROUND);
  tft->setTextSize(3);
  tft->setCursor(20, 30);
  tft->print(str); 
}

void printButton(Adafruit_TFTLCD *tft, const char * str, uint8_t x, uint8_t y) {
  tft->setTextColor(TEXT, BACKGROUND);
  tft->setTextSize(2);
  tft->setCursor(10 + 120 * y, 110 + 80 * x );
  tft->print(str);
}

void setup() {
  // put your setup code here, to run once:

  tft.reset();

  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
  } else if(identifier == 0x9328) {
    //Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    //Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    //Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    //Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier==0x0101) {     
      identifier=0x9341;
  } else {
    identifier=0x9341;
   
  }


  uint16_t background = 0x0680;
  uint16_t border = 0x0019;
  
  tft.begin(identifier);
  tft.setRotation(2);

  drawLayOut(&tft);
  printTitle(&tft, "Hello");

}

void loop() {
  // put your main code here, to run repeatedly:
  clearLayout(&tft);
  printTitle(&tft, "WHAT?");
  printButton(&tft, "THEY'RE", 0, 0);
  printButton(&tft, "HOLD ON", 0, 1);
  printButton(&tft, "YOU ARE", 1, 0);
  printButton(&tft, "THEY ARE", 1, 1);
  printButton(&tft, "Five", 2, 0);
  printButton(&tft, "Six", 2, 1);
  delay(10000);
}
