#include <elapsedMillis.h>
#include <esp_now.h>
#include <WiFi.h>
#include "common.h"


//HERE YOU CAN EDIT STUFF LAURA 

//This is the default speed of breathing
//best is somewhere between 2 and 40
int breathingBPM = 5;
int defaultBreathingBPMWithoutHumanDetected = 5;


//This is the base position on the hue wheel (0.5 is a kind of blue)
//see here https://learn.adafruit.com/assets/74094
float defaultHue = 0.5;
//Hue range is the amount of hue it goes up every breathing cycle 
//(hue + hueRange cannot be more than 1 otherwise, I dont know what will happen)
float hueRange = 0.2; 
//this is the minimum brightness the LEDs can get, should be somewhere between 0 and 255
int LEDminimumBrightness = 50;

//lower values (between 0 and 1) make the brightness curve of the LEDs steeper
float LEDPower = 0.4;

//STOP EDITING BELOW THIS
// VERY DANGEROUS 
//NO INSURANCE WILL COVER IF YOU EDIT BELOW THIS
//AFBLIJVEN LAURA
//AFBLIJVEN LAURA
//AFBLIJVEN LAURA
//AFBLIJVEN LAURA
//AFBLIJVEN LAURA

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int heartbeatRate;
  bool humanPresence;
  int breathingsPerMinute;
} struct_message;

// Create a struct_message called myData
struct_message receivedData;


//breath cycle variables
elapsedMillis timeSinceLastBreathCycleStart;
float breathingInMotorValue = 0;
float breathingOutMotorValue = 0;
float breathCyclePercentage = 0;
float breathCycleSineWave = 0;
int numBreathingCycles = 0;
bool breathingIn = false;
bool _breathingIn = false;
int msPerBreathCycle = 60000 / breathingBPM;


//Touch values
float touchValue = 0;
int touchBaseLevel = 0;
int touchThreshold = 5;
bool beingTouched = false;

//debug values:
long cycleCount = 0;
#define LED_BUILTIN 2

//ESPNow data
bool humanPresence = true;

//switch values
int switchState = 0;  //-1 is left, 0 is middle, 1 is right
int _switchState = 0; 

//LED values
int LEDBrightness = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("I am living");

  pinMode(LED_BUILTIN, OUTPUT);
  showMacAdresses();
  setupBreathing();
  setupTouch();
  setupSwitch();
  setupLED();
  setupESPNow();

  showMacAdresses();
  
  Serial.println("All setups done, starting life");
  }

void loop() {

  updateBreathing();
  updateTouch();
  updateSwitch();
  updateLED();

  checkSerial();

  // printDebug();
}
