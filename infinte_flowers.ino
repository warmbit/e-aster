/* 
 *  e-aster: inifinite flowers
 *  
 *  flowers of different colors emerges one after another.
 *  
 *  this effect gives one illusion:
 *  waves at the center up and down and ripple out
 *    
 *  - using concentric circles of e-aster
 *  - arrange the palette with contrasting colors in neighbors
 *  - use nblendPaletteTowardPalette() to cross-fade palettes
 *  
 *  @warmbit 6-19-2021
 *  
 */
 
#include "FastLED.h"

// TODO
// change your DATA-IN and NUM_LEDS accordingly
#define DATA_PIN    A1
#define NUM_LEDS    61 // 30
#define BRIGHTNESS  64
#define MIN_BRIGHTNESS  8
#define MAX_BRIGHTNESS  64 
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

// TODO
// adjust speed for different effects
#define UPDATES_PER_SECOND 40 // 20 // 100 //50 //20

//
// this sketch is adapted from Mark's post.
//
// This example shows how to cross-fade between different color palettes
// using the function nblendPaletteTowardPalette.
//
// The basic idea is that you always have a "current palette" that you're
// pulling colors from with ColorFromPalette, and you have a "target palette"
// which is the 'next' palette that you want to get to.
// 
// After that, implementation is relatively simple: just periodically call
//   nblendPaletteTowardPalette( currentPalette, targetPalette);
// If the current palette is not yet equal to the target palette, this 
// function will make a few small changes to the current palette to make
// it slightly more like the target.  Over time, the current palette will
// come to be equal to the target.
// There's no need to test the current and target for equality; it's safe
// to keep calling nblendPaletteTowardPalette even after current reaches target.
// For faster blending, call nblendPaletteTowardPalette twice per loop.
//

CRGBPalette16 currentPalette( CRGB::Black);
CRGBPalette16 targetPalette( PartyColors_p );

// aster specific
// rings
const uint8_t rings[6][12] = {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},{ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12},{13,14,15,16,17,18,19,20,21,22,23,24},{25,26,27,28,29,30,31,32,33,34,35,36},{37,39,41,43,45,47,49,51,53,55,57,59},{38,40,42,44,46,48,50,52,54,56,58,60}};

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop()
{
  ChangePalettePeriodically();

  // Crossfade current palette slowly toward the target palette
  //
  // Each time that nblendPaletteTowardPalette is called, small changes
  // are made to currentPalette to bring it closer to matching targetPalette.
  // You can control how many changes are made in each call:
  //   - the default of 24 is a good balance
  //   - meaningful values are 1-48.  1=veeeeeeeery slow, 48=quickest
  //   - "0" means do not change the currentPalette at all; freeze
  
  // TODO
  // another place to adjust the speed.
  uint8_t maxChanges = 24; 
  nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);


  static uint8_t startIndex = 0;
  
  // TODO
  //FillLEDRingsFromPaletteColors(startIndex);
  FillLEDRings2FromPaletteColors(startIndex);
  
  startIndex = startIndex + 1; /* motion speed */
  
  FastLED.show();
  FastLED.delay(1000/UPDATES_PER_SECOND);
}

// how to fill the buffer with palette colors
void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex + sin8(i*16), brightness);
    colorIndex += 3;
  }
}

void FillLEDRingsFromPaletteColors( uint8_t colorIndex)
{
    // LINEARBLEND is better than NOBLEND, it makes the animation smoother.
    uint8_t i, j, index;
    CRGB c1 = ColorFromPalette( currentPalette, colorIndex, MAX_BRIGHTNESS, LINEARBLEND);
    CRGB c2 = ColorFromPalette( currentPalette, (colorIndex+4)%256, MAX_BRIGHTNESS, LINEARBLEND);
    CRGB c3 = ColorFromPalette( currentPalette, (colorIndex+8)%256, MAX_BRIGHTNESS, LINEARBLEND);
    CRGB c4 = ColorFromPalette( currentPalette, (colorIndex+16)%256, MAX_BRIGHTNESS, LINEARBLEND);

    for (i = 0; i < 6; i++) {
      for (j = 0; j < 12; j++) {
        index = rings[i][j];
        switch(i) {
          case 0:
            leds[index] = c1;
            break;
          case 1: 
            leds[index] = c1;
            break;
          case 2:
            leds[index] = c2;
            break;
          case 3: 
            leds[index] = c3;
            break;
          case 4:
            leds[index] = c4;
            break;
          default:
            leds[index] = CRGB::Gray;
            break;
        }
      }
      colorIndex += 2; // 4, small steps better?
    }
}

// reverse the direction from center to edge
void FillLEDRings2FromPaletteColors( uint8_t colorIndex)
{
    // LINEARBLEND is better than NOBLEND
    uint8_t i, j, index;
    CRGB c1 = ColorFromPalette( currentPalette, colorIndex, MAX_BRIGHTNESS, LINEARBLEND);
    CRGB c2 = ColorFromPalette( currentPalette, (colorIndex+4)%256, MAX_BRIGHTNESS, LINEARBLEND);
    CRGB c3 = ColorFromPalette( currentPalette, (colorIndex+8)%256, MAX_BRIGHTNESS, LINEARBLEND);
    CRGB c4 = ColorFromPalette( currentPalette, (colorIndex+16)%256, MAX_BRIGHTNESS, LINEARBLEND);

    for (i = 0; i < 6; i++) {
      for (j = 0; j < 12; j++) {
        index = rings[i][j];
        switch(i) {
          case 0:
            leds[index] = c4;
            break;
          case 1: 
            leds[index] = c4;
            break;
          case 2:
            leds[index] = c3;
            break;
          case 3: 
            leds[index] = c2;
            break;
          case 4:
            leds[index] = c1;
            break;
          default:
            leds[index] = c4;
            break;
        }
      }
      colorIndex += 2; // 4, small steps better?
    }
}
// TODO
// add new palette here to experiment
//
void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 120;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    CRGB r = CHSV( HUE_RED, 255, 255);
    CRGB p = CHSV( HUE_PURPLE, 255, 255);
    CRGB y = CHSV( HUE_YELLOW, 255, 255);
    CRGB o = CHSV( HUE_ORANGE, 255, 255);
    CRGB g = CHSV( HUE_GREEN, 255, 255);
    CRGB b = CRGB::Blue;
    CRGB black = CRGB::Black;
    CRGB w = CRGB::White;
    CRGB gray = CRGB::Gray;
    
    if( secondHand ==  0)  { targetPalette = RainbowColors_p; }
    if( secondHand == 20)  { targetPalette = CRGBPalette16( r,r,y,y, p,p,o,o, g,g,b,b, r,g,b,black); }
    if( secondHand == 40)  { targetPalette = CRGBPalette16( black,w,black,w, r,y,b,p, p,b,o,y, g,r,g,r); }
    //if( secondHand == 60)  { targetPalette = LavaColors_p; }
    if( secondHand == 60)  { targetPalette = CRGBPalette16( black,gray,black,r, r,gray,b,gray, p,w,o,w, g,w,g,w); }
    //if( secondHand == 80)  { targetPalette = CloudColors_p; }
    if( secondHand == 80)  { targetPalette = CRGBPalette16( r,black,p,w, black,y,g,o, r,gray,b,o, black,r,g,r); }
    //if( secondHand == 100)  { targetPalette = PartyColors_p; }
    if( secondHand == 100)  { targetPalette = CRGBPalette16( r,b,y,b, p,o,b,r, g,r,b,r, r,g,b,black); }
    
  }
}
