#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define MID_PIN 10
#define LEFT_PIN 6
#define RIGHT_PIN 3

#define FRONT_NUM_LEDS 11
#define MID_NUM_LEDS 28

#define FRONT_BRIGHTNESS 75
#define MID_BRIGHTNESS 80

int gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

Adafruit_NeoPixel l_strip = Adafruit_NeoPixel(FRONT_NUM_LEDS, LEFT_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel r_strip = Adafruit_NeoPixel(FRONT_NUM_LEDS, RIGHT_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel mid_strip = Adafruit_NeoPixel(MID_NUM_LEDS, MID_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  // Setup Left strip
  l_strip.setBrightness(FRONT_BRIGHTNESS);
  l_strip.begin();
  l_strip.show(); // Initialize all pixels to 'off'

  // Setup Right strip
  r_strip.setBrightness(FRONT_BRIGHTNESS);
  r_strip.begin();
  r_strip.show(); // Initialize all pixels to 'off'

  // Setup Right strip
  mid_strip.setBrightness(MID_BRIGHTNESS);
  mid_strip.begin();
  mid_strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  setAllPixels(l_strip, 128, 128, 128, 255);
  setAllPixels(r_strip, 128, 128, 128, 255);
  theaterChaseRainbow(mid_strip, 50);
}


void setAllPixels(Adafruit_NeoPixel strip, uint8_t r, uint8_t g, uint8_t b, uint8_t gma) {
  for (uint16_t i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(r,g,b, gamma[gma] ) );
  }
  strip.show();
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(Adafruit_NeoPixel strip, uint8_t wait) {
  while (true) {
    for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
      for (int q=0; q < 3; q++) {
        for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(strip.numPixels()-i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();

        delay(wait);

        for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return l_strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return l_strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return l_strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}
