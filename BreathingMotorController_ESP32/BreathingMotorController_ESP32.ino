#include <elapsedMillis.h>
#include <esp_now.h>
#include <WiFi.h>
#include "common.h"
#include "LPFilter.hpp"


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
int LEDminimumBrightness = 140;

//lower values (between 0 and 1) make the brightness curve of the LEDs steeper
float LEDPower = 0.4;


//the percentage of breathing in and out (between 100 and 0)
float breathingInAmplitude = 100.0;
float breathingOutAmplitude = 85.0;

float breathingDivision = 0.5;  //0.5 means breathing in and out is same length
//0.6 means breathing in for 60% of the time, breathing out for 40%
//0.4 means breathing in for 40% of the time, breathing out for 60% etc


//This is the name of the pod so it listens to the correct messages
#define POD_IDENTIFIER 'a'  //either 'a' or 'b'
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

elapsedMillis timeSinceESPNowSent;
elapsedMillis timeSinceTouchMotorsUpdated;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("I am living");

  pinMode(LED_BUILTIN, OUTPUT);
  showMacAdresses();
  setupBreathing();
  setupTouch();
  setupSwitch();
  // setupLED();
  setupESPNow();

  showMacAdresses();

  Serial.println("All setups done, starting life");
}

void loop() {

  updateBreathing();
  // updateTouch();
  if (timeSinceTouchMotorsUpdated > 1) {
    //make sure this doesnt get called more than once per ms
    updateTouchMotors();
  }
  updateSwitch();
  // updateLED();

  //send data every 500 ms
  if (timeSinceESPNowSent > 20) {
    sendData();
    timeSinceESPNowSent = 0;
  }
  checkSerial();

  // printDebug();
}
