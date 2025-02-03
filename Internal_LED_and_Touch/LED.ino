#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif



// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 18

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 64

int LEDHue = 0;
int saturation = 180;
float fadeInPercentage = 0.0;

bool stressedState = false;
bool excitedState = false;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setupLED() {

  strip.begin();             // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();              // Turn OFF all pixels ASAP
  strip.setBrightness(255);  // Set BRIGHTNESS to about 1/5 (max = 255)
  pinMode(LED_BUILTIN, OUTPUT);
}

void updateLED() {

  //LAURA, here you can change the BPMS that will trigger a certain state
  String state = "neutral";
  int heartbeatRate = receivedData.heartbeatRate;
  if (heartbeatRate > 80 && heartbeatRate < 100) {

    //this is the excited state
    excitedState = true;
    stressedState = false;
    state = "excited";
  } else if (heartbeatRate > 100) {
    //this is the stressed state
    excitedState = false;
    stressedState = true;
    state = "stressed";
  } else {
    //this is default state
    excitedState = false;
    stressedState = false;
  }


  Serial.println("State is " + String(state));

    if (humanPresence) {
    //show the led when there seems to be someone close to the sensor
    digitalWrite(LED_BUILTIN, HIGH);
    fadeInPercentage = constrain(fadeInPercentage + 0.01, 0.0, 1.0);
    // Serial.println("FadeInPercentage:" + String(fadeInPercentage));
    for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
      int hueWheelSize = 65536;

      saturation = 180;
      //the whole animation depends on the breathCyclePercentage value
      float breathCycleSineWaveLED = abs(sin(breathCyclePercentage * TWO_PI));
      //make it a steeper sloper
      breathCycleSineWaveLED = pow(breathCycleSineWaveLED, LEDPower);

      //pick the right hue for the current state standard hue
      float hue = defaultHue;
      //if we are excited, pick the excited hue
      if (excitedState) {
        hue = excitedHue;
      } else if (stressedState) {
        //if we are stressed pick the stressed hue
        hue = stressedHue;
      }
      //make the hue change over time
      hue += (breathCycleSineWaveLED * hueRange);

      //make sure it wraps back to 0 if it overflows
      hue = hue % 1.0;

      //convert the floating hue to a hue value on the hue wheel
      LEDHue = int(float(hueWheelSize) * hue);

      //convert it to LED values
      LEDBrightness = breathCycleSineWaveLED * 255.;

      //slowly fade the LEDs in when we detect a human again
      // LEDBrightness = LEDBrightness * fadeInPercentage;


      //make sure the LEDs never fully turn off by setting a minimum of 10 for the brightness
      LEDBrightness = constrain(LEDBrightness, LEDminimumBrightness, 255);

      //calculate an LED color
      uint32_t rgbcolor = strip.ColorHSV(LEDHue, saturation, LEDBrightness * fadeInPercentage);

      //fill the strip with it
      strip.fill(rgbcolor);
      strip.show();  //  Update strip to match
    }

    Serial.println("Hue is " + String(hue));
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    //slowly fade the LED strip out
    LEDBrightness -= 0.1;
    LEDBrightness = constrain(LEDBrightness, 0, 255);
    uint32_t rgbcolor = strip.ColorHSV(LEDHue, saturation, LEDBrightness);
    fadeInPercentage = 0.0;  //make sure we fade in when we get data next time

    strip.fill(rgbcolor);
    strip.show();
  }
}