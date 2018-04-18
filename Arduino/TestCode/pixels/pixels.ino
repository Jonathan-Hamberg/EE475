#include <Adafruit_NeoPixel.h>


#define PIN 13

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(36, PIN, NEO_GRB + NEO_KHZ800);
    
uint32_t red;
uint32_t green;
uint32_t blue;
uint32_t yellow;
uint32_t white;

void setup() {
  Serial.begin(9600);
  strip.begin();
  //strip.show(); // Initialize all pixels to 'off'
  red = strip.Color(10, 0, 0);
  green = strip.Color(0, 10, 0);
  blue = strip.Color(0, 0, 10);
  yellow = strip.Color(10, 10, 0);
  white = strip.Color(10, 10, 10);

  Serial.print("Red: ");
  Serial.println(red);
  Serial.print("Green: ");
  Serial.println(green);
  Serial.print("Blue: ");
  Serial.println(blue);
  Serial.print("Yellow: ");
  Serial.println(yellow);
  Serial.print("White: ");
  Serial.println(white);
}


void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 36; i++) {
    strip.setPixelColor(i, red);
  }
  strip.show();
  delay(1000);
  for (int i = 0; i < 36; i++) {
    strip.setPixelColor(i, yellow);
  }
  strip.show();
  delay(1000);
  for (int i = 0; i < 36; i++) {
    strip.setPixelColor(i, green);
  }
  strip.show();
  delay(1000);
  for (int i = 0; i < 36; i++) {
    strip.setPixelColor(i, blue);
  }
  strip.show();
  delay(1000);
  for (int i = 0; i < 36; i++) {
    strip.setPixelColor(i, white);
  }
  strip.show();
  delay(1000);
}
