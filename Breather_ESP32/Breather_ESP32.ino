#include <elapsedMillis.h>
#include <esp_now.h>
#include <WiFi.h>
#include "common.h"

// common.h
#ifndef COMMON_H
#define COMMON_H

typedef struct struct_message {
  bool beingTouched;
} struct_message;

#endif


// Create a struct_message called myData
struct_message receivedData;


//breath cycle variables
elapsedMillis timeSinceLastBreathCycleStart;
int breathingBPM = 5;
float breathingInMotorValue = 0;
float breathingOutMotorValue = 0;
float breathCyclePercentage = 0;
float breathCycleSineWave = 0;
bool breathingIn = false;
int msPerBreathCycle = 60000 / breathingBPM;


//Touch values
float touchValue = 0;
int touchBaseLevel = 0;
int touchThreshold = 5;
bool beingTouched = false;

//debug values:
long cycleCount = 0;
#define LED_BUILTIN 2


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("I am living");

  pinMode(LED_BUILTIN, OUTPUT);
  showMacAdresses();
  setupBreathing();
  setupTouch();
}

void loop() {
  // put your main code here, to run repeatedly:
  cycleCount++;
  updateBreathing();
  updateTouch();
  checkSerial();

  printDebug();
}
