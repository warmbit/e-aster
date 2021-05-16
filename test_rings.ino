#include "FastLED.h"

#define DATA_PIN    A1
#define NUM_LEDS    61
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB // Green and Red is in wrong order.
CRGB leds[NUM_LEDS];

void setup() {
  delay( 3000 );
  // put your setup code here, to run once:
  pinMode(DATA_PIN, OUTPUT);
  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
}

const uint8_t rings[6][12] = {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},{ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12},{13,14,15,16,17,18,19,20,21,22,23,24},{25,26,27,28,29,30,31,32,33,34,35,36},{37,39,41,43,45,47,49,51,53,55,57,59},{38,40,42,44,46,48,50,52,54,56,58,60}};
const CRGB colors[3] = {CRGB::Green, CRGB::Red, CRGB::Blue};
 
void loop() {
  // put your main code here, to run repeatedly:
  // RED is green
  // Green is red
  // Blue is blue
  
  for (uint8_t c = 0; c < 3; c++)
  {
  uint8_t index = 0;
  CRGB color = colors[c];
     
  for (uint8_t i = 0; i < 6; i++)
  {
    for (uint8_t j = 0; j < 12; j++) 
    {
      index = rings[i][j];    
      leds[index] = color;
    }
    FastLED.show(); 
    delay(500);
  
    for (uint8_t k = 0; k < 12; k++) 
    {
      index = rings[i][k];     
      leds[index] = CRGB::Black;
    }
    FastLED.show(); 
    delay(500);
  }

  delay(1000);
  }
}
