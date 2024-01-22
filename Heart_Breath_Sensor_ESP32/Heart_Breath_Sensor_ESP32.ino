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
uint8_t broadcastAddress[] = { 0x64, 0xB7, 0x08, 0xB8, 0x37, 0xA0 };


// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


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
    // Serial.println("No one is here it seems...");
    messageToSend.humanPresence = false;
    sendESPNowMessage();
  } else if (messageToSend.heartbeatRate > 0 || messageToSend.breathingsPerMinute > 0) {
    messageToSend.humanPresence = true;
  }
  //show the led when there seems to be someone close to the sensor
  digitalWrite(LED_BUILTIN, messageToSend.humanPresence);

  // Serial.println("Heart_rate_monitoring_system_acitve");
  HR_BR();
  delay(100);
}