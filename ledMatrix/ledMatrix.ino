#include <Adafruit_NeoPixel.h>

#define LED_PIN 6 
#define NUM_LEDS 28
#define POT_PIN A1

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int smoothed = 0;

void setup() {
  strip.begin();
  strip.setBrightness(100);   
  strip.show();               
}

void loop() {
  int raw = analogRead(POT_PIN);          
  smoothed = (smoothed * 3 + raw) / 4;    

  int numLit = map(smoothed, 0, 1023, 0, NUM_LEDS);

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < numLit) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));  
    } else {
      strip.setPixelColor(i, 0);                           
    }
  }
  strip.show();
  delay(10);
}
