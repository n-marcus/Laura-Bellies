#include <elapsedMillis.h>
#include <esp_now.h>
#include <WiFi.h>
#include "common.h"
#include "LPFilter.hpp"


//HERE YOU CAN EDIT STUFF LAURA
int breathingBPM = 5;
int defaultBreathingBPMWithoutHumanDetected = 5;
bool shouldBeBreathing = false;

//This is the base position on the hue wheel (0.5 is a kind of blue)
//see here https://learn.adafruit.com/assets/74094
float defaultHue = 0.3;  // 0.5
float excitedHue = 0.7;  // 0.2
float stressedHue = 0.9;

//Hue range is the amount of hue it goes up every breathing cycle
//(hue + hueRange cannot be more than 1 otherwise, I dont know what will happen)
float hueRange = 0.2;
//this is the minimum brightness the LEDs can get, should be somewhere between 0 and 255
int LEDminimumBrightness = 100;  //140 was the default

//lower values (between 0 and 1) make the brightness curve of the LEDs steeper
float LEDPower = 0.4;



//This is the name of the pod so it listens to the correct messages
#define POD_IDENTIFIER 'b'  //either 'a' or 'b'
//a is BLACK
//b is RED

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
  char target;
  int heartbeatRate;
  bool humanPresence;
  int breathingsPerMinute;
  bool beingTouched;
  bool isAboutTouch;
  float breathingPercentage;
} struct_message;

// Create a struct_message called myData
struct_message receivedData;
struct_message messageToSend;


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

int messagesReceived = 0;
int messagesSend = 0;


//Touch values
float touchValue = 0;
int touchBaseLevel = 0;
int touchThreshold = 3;
bool beingTouched = false;
bool _beingTouched = false;

//debug values:
long cycleCount = 0;
#define LED_BUILTIN 2

//ESPNow data
bool humanPresence = true;

//switch values
int switchState = 0;  //-1 is left, 0 is middle, 1 is right
int _switchState = 0;

//LED values
float LEDBrightness = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("I am living");

  pinMode(LED_BUILTIN, OUTPUT);
  showMacAdresses();
  // setupBreathing();
  setupTouch();
  // setupSwitch();
  setupLED();
  setupESPNow();

  showMacAdresses();

  Serial.println("All setups done, starting life");
}

elapsedMillis functionUpdateMs;
elapsedMillis touchUpdateMs;

void loop() {

  if (touchUpdateMs > 50) {
    updateTouch();
  }

  if (functionUpdateMs > 8) {
    // functionUpdateMs = 0;
    updateLED();
    // Serial.println("LED update took: " + String(functionUpdateMs) + "ms");

    // functionUpdateMs = 0;
    checkSerial();
    functionUpdateMs = 0;
  }
  // Serial.println("Serial update took: " + String(functionUpdateMs) + "ms");
  // printDebug();
}
