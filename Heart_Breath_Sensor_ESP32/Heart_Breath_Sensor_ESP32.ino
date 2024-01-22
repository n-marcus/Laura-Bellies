#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <esp_now.h>
#include <elapsedMillis.h>

#define RXD2 16
#define TXD2 17
#include <60ghzbreathheart.h>

#define LED_BUILTIN 2

BreathHeart_60GHz radar = BreathHeart_60GHz(&Serial2);
unsigned long delay_Intial = 0;
const long delay_interval = 1000;
unsigned long measure_Intial = 0;
const long measure_interval = 500;
int Human_pesence_pin = 27;
int Human_presence;
int Heart_rate_measure_button_pin = 26;


elapsedMillis timeSinceLastSuccesfulReading;
int maxTimeSinceLastSucessfulReading = 10000;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int heartbeatRate;
  bool humanPresence;
  int breathingsPerMinute;
} struct_message;

// Create a struct_message called myData
struct_message messageToSend;

esp_now_peer_info_t peerInfo;

// REPLACE WITH YOUR RECEIVER MAC Address
// uint8_t broadcastAddress[] = { 0x64, 0xB7, 0x08, 0xB8, 0x37, 0xA0 };
// uint8_t broadcastAddress[] = { 0x48, 0xE7, 0x29, 0xB6, 0x74, 0x30 };
uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };




void _delay() {
  delay_Intial = millis();
  while ((millis() - delay_Intial) <= delay_interval) {
  }
}


void setup() {
  Serial.begin(115200);                           //RX0,TX0 OF ESP32
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  //RX2,TX2 OF ESP32
  pinMode(Heart_rate_measure_button_pin, INPUT);
  pinMode(Human_pesence_pin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  setupESPNow();
}

void loop() {
  // Heart_Button = digitalRead(Heart_rate_measure_button_pin);
  // IR_Status = digitalRead(IR_Sensor_pin);
  Human_presence = digitalRead(Human_pesence_pin);

  if (timeSinceLastSuccesfulReading > maxTimeSinceLastSucessfulReading || (messageToSend.heartbeatRate == 0 && messageToSend.breathingsPerMinute == 0)) {
    //if more than 10 seconds passed without a succesful reading, or if both the two sensor readings returned zero, we agree there is noone here
    messageToSend.humanPresence = false;

    //send update immediately to turn the heartbeat off
    sendESPNowMessage();
  } else if (messageToSend.heartbeatRate > 0 || messageToSend.breathingsPerMinute > 0) {
    //if any of the sensor readings have been succesful, we can agree there is someoen in front of the sensor
    messageToSend.humanPresence = true;
  }
  //show the led when there seems to be someone close to the sensor
  digitalWrite(LED_BUILTIN, messageToSend.humanPresence);

  // Serial.println("Heart_rate_monitoring_system_acitve");
  HR_BR();
  delay(100);
}