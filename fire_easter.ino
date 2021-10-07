/*
 * Fire-easter concentric style
 * adapted from fire_snow2
 * 
 * warmbit @ 2021OCT07
 * 
 * original ref: sun radiation circular
 * https://wokwi.com/arduino/projects/288437917385228808
 * 
 */

#include "FastLED.h"

#define LED_PIN       A1
#define COLOR_ORDER   GRB
#define LED_TYPE      WS2812B

#define BRIGHTNESS    255

// just for mapping
#define NUM_ROWS 17 
#define NUM_COLS 17
const uint8_t kMatrixWidth = NUM_COLS;
const uint8_t kMatrixHeight = NUM_ROWS;

//#define NUM_LEDS (NUM_ROWS * NUM_COLS)
#define NUM_LEDS 61

CRGB leds[NUM_LEDS];
byte bump[(NUM_COLS+2)*(NUM_ROWS+2)];
CRGB cHsvLUT[256];

void setup() {
  
  delay(3000); // sanity delay
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  generateCHsvLUT();
}

void loop() {
  generateBump();
  bumpMap();
  FastLED.show();
}

void generateCHsvLUT() {
  for (int j= 0; j<256; j++) {
    cHsvLUT[j] = HeatColor(j/1.4);
  }
}

void generateBump () {
  int t = millis()/2;
  int index = 0;
  for (byte j = 0; j < (NUM_ROWS+2); j++) {
    for (byte i = 0; i < (NUM_COLS+2); i++) {                     
      byte col = (inoise8_raw(i*40, j*40, t))/2; 
      bump[index++] = col;
    } 
  }
} 

void bumpMap() {
  int yindex=(NUM_COLS+3);
  int8_t vly=-(NUM_ROWS/2+1);
  for (byte y = 0; y < NUM_ROWS; y++) { 
    ++vly;
    int8_t vlx=-(NUM_COLS/2+1);
    for (byte x = 0; x < NUM_COLS; x++) {
      ++vlx;
      int8_t nx=bump[x+yindex+1]-bump[x+yindex-1];
      int8_t ny=bump[x+yindex+(NUM_COLS+2)]-bump[x+yindex-(NUM_COLS+2)];
      byte difx=abs8(vlx*7-nx);
      byte dify=abs8(vly*7-ny);
      int temp = difx*difx+dify*dify;
      
      int col = 255-temp/13;
      //int col = 255-temp/11;
      // reverse: edge hotter, inner darker
      //int col = temp/11;
      
      if (col<0) col=0;
      
      uint8_t index = XY(x, y);
      if (index != 61) {
        leds[index] = cHsvLUT[col];
      }
    }
    yindex += (NUM_COLS+2);
  }
}

uint8_t XY (uint8_t x, uint8_t y) {
  // 61 is a guard, there is no actual LED there for snowflake
  // if 61 returned, it is NOT visible at all.
  //
  // this is a mapping from xy matrix for e-aster
  const uint8_t XYTable[] = {
    61,  61,  61,  61,  61,  61,  61,  61,  38,  61,  61,  61,  61,  61,  61,  61,  61,
    61,  61,  61,  61,  61,  60,  61,  37,  61,  39,  61,  40,  61,  61,  61,  61,  61,
    61,  61,  61,  61,  61,  61,  61,  61,  25,  61,  61,  61,  61,  61,  61,  61,  61,
    61,  61,  61,  61,  59,  61,  36,  61,  61,  61,  61,  26,  61,  41,  61,  61,  61,
    61,  58,  61,  61,  61,  61,  61,  24,  61,  13,  61,  61,  61,  61,  61,  42,  61,
    61,  61,  61,  35,  61,  23,  61,  61,  61,  61,  61,  14,  61,  27,  61,  61,  61,
    61,  57,  61,  61,  61,  61,  61,  11,  12,   1,  61,  61,  61,  61,  61,  43,  61,
    61,  61,  61,  61,  22,  61,  10,  61,  61,  61,   2,  61,  15,  61,  61,  61,  61,
    56,  61,  34,  61,  61,  61,   9,  61,   0,  61,   3,  61,  61,  61,  28,  61,  44,
    61,  61,  61,  61,  21,  61,   8,  61,  61,  61,   4,  61,  16,  61,  61,  61,  61,
    61,  55,  61,  61,  61,  61,  61,   7,   6,   5,  61,  61,  61,  61,  61,  45,  61,
    61,  61,  61,  33,  61,  20,  61,  61,  61,  61,  61,  17,  61,  29,  61,  61,  61,
    61,  54,  61,  61,  61,  61,  61,  19,  61,  18,  61,  61,  61,  61,  61,  46,  61,
    61,  61,  61,  53,  61,  32,  61,  61,  61,  61,  61,  30,  61,  47,  61,  61,  61,
    61,  61,  61,  61,  61,  61,  61,  61,  31,  61,  61,  61,  61,  61,  61,  61,  61,
    61,  61,  61,  61,  52,  61,  51,  61,  61,  61,  49,  61,  48,  61,  61,  61,  61,
    61,  61,  61,  61,  61,  61,  61,  61,  50,  61,  61,  61,  61,  61,  61,  61,  61
  };

  uint8_t i = (y * kMatrixWidth) + x;
  uint8_t j = XYTable[i];
  return j;
}
