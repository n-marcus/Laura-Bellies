#define NUM_LEDS 22
#define LED_PIN 25

DEFINE_GRADIENT_PALETTE(cw1_002_gp){
  0, 20, 50, 80,
  100, 120, 200, 255,
  200, 255, 150, 80,
  200, 190, 50, 22,
  255, 255, 0, 0
};

CRGBPalette16 MyPalette = cw1_002_gp;

CRGB leds[NUM_LEDS];

void setupLEDs() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

void updateLEDs() {
  // int paletteIndex = map(myData.gsr_average, 0, 2000, 0, 255);
  // fill_solid(leds, NUM_LEDS, ColorFromPalette(MyPalette, paletteIndex));
  FastLED.show();
}