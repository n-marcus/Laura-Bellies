#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif



// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 18

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 23

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setupLED() {

  strip.begin();             // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();              // Turn OFF all pixels ASAP
  strip.setBrightness(150);  // Set BRIGHTNESS to about 1/5 (max = 255)
}

void updateLED() {
  if (humanPresence) {
    for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
      int hueWheelSize = 65536;

      int saturation = 180;
      float breathCycleSineWaveLED = abs(sin(breathCyclePercentage * TWO_PI));
      //make it a steeper sloper
      breathCycleSineWaveLED = pow(breathCycleSineWaveLED, LEDPower);

      float hue = defaultHue;
      //make the hue change over time
      hue += (breathCycleSineWaveLED * hueRange);

      //convert the floating hue to a hue value on the hue wheel
      int LEDHue = int(float(hueWheelSize) * hue);

      //convert it to LED values
      LEDBrightness = breathCycleSineWaveLED * 255.;

      //make sure the LEDs never fully turn off by setting a minimum of 10 for the brightness
      LEDBrightness = constrain(LEDBrightness, LEDminimumBrightness, 255);



      //calculate an LED color
      uint32_t rgbcolor = strip.ColorHSV(LEDHue, saturation, LEDBrightness);

      //fill the strip with it
      strip.fill(rgbcolor);
      strip.show();  //  Update strip to match
    }
  } else {
    strip.fill(strip.ColorHSV(0,0,0));
    strip.show();
  }
}