/*
 * Fire-easter XY style
 * adapted from fire_snow
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

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND   60

#define NUM_ROWS 17 
#define NUM_COLS 17
const uint8_t kMatrixWidth = NUM_COLS;
const uint8_t kMatrixHeight = NUM_ROWS;

#define SNOW_LEDS     61
#define NUM_SNOW      1
#define NUM_LEDS      (NUM_SNOW*SNOW_LEDS)

// fire simulation uses this!
bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

// Fire2012 with programmable Color Palette
//
// This code is the same fire simulation as the original "Fire2012",
// but each heat cell's temperature is translated to color through a FastLED
// programmable color palette, instead of through the "HeatColor(...)" function.
//
// Four different static color palettes are provided here, plus one dynamic one.
// 
// The three static ones are: 
//   1. the FastLED built-in HeatColors_p -- this is the default, and it looks
//      pretty much exactly like the original Fire2012.
//
//  To use any of the other palettes below, just "uncomment" the corresponding code.
//
//   2. a gradient from black to red to yellow to white, which is
//      visually similar to the HeatColors_p, and helps to illustrate
//      what the 'heat colors' palette is actually doing,
//   3. a similar gradient, but in blue colors rather than red ones,
//      i.e. from black to blue to aqua to white, which results in
//      an "icy blue" fire effect,
//   4. a simplified three-step gradient, from black to red to white, just to show
//      that these gradients need not have four components; two or
//      three are possible, too, even if they don't look quite as nice for fire.
//
// The dynamic palette shows how you can change the basic 'hue' of the
// color palette every time through the loop, producing "rainbow fire".

CRGBPalette16 gPal;

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  // This first palette is the basic 'black body radiation' colors,
  // which run from black to red to bright yellow to white.
  gPal = HeatColors_p;

}

void loop() {
  int  a = millis();
  int  a1 = a/3;
  for (uint8_t j = 0; j < NUM_ROWS; j++) { 
    for (uint8_t i = 0; i < NUM_COLS; i++) {
      uint16_t ledIndex = XY(i,j);
      if (ledIndex != 61) {
        for (uint8_t n = 0; n < NUM_SNOW; n++) {
          leds[n*NUM_SNOW+ledIndex] = HeatColor(qsub8 (inoise8 (i * 50, j * 50+a, a1), abs8(j - (NUM_ROWS-1)) * 255 / (NUM_ROWS+20)));
        }
      }
    }
  }

  FastLED.show();
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
