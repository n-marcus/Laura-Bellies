#include <elapsedMillis.h>
#include <esp_now.h>
#include <WiFi.h>
#include <FastLED.h>
#include <driver/i2s.h>
#include "M5Atom.h"

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  char target;
  int heartbeatRate;
  bool humanPresence;
  int breathingsPerMinute;
  bool beingTouched;
} struct_message;

// Create a struct_message called myData to hold the received data
struct_message myData;

int heartbeatRate = 60;  //default heartbeat rate in bpm
bool humanPresence = true;
int currentInterval = 0;

#define POD_IDENTIFIER 'b'


#define RED_LED_PIN 10

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);


  M5.begin(true, false, true);
  M5.dis.clear();
  M5.dis.drawpix(0, CRGB(128, 0, 0));

  setupLEDs();
  setupSpeaker();
  setupESPNow();
  delay(100);
}


void loop() {
  updateSound();
  updateLEDs();
  checkSerial();
  M5.update();  // You can still call M5.update() if needed
}
