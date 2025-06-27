#include <esp_now.h>
#include <WiFi.h>
#include <Stepper.h>

// === Force Sensor ===
#define FORCE_SENSOR_PIN 36

// === Motor 1 (Local) ===
#define MOTOR1IN1 13
#define MOTOR1IN2 12
#define MOTOR1IN3 14
#define MOTOR1IN4 27

// === Motor 2 (Remote) ===
#define MOTOR2IN1 18
#define MOTOR2IN2 19
#define MOTOR2IN3 22
#define MOTOR2IN4 23

// === Stepper Setup ===
const int stepsPerRevolution = 2048;
const int stepsFor2Seconds = 341;
const int stepsFor3Seconds = 512;
const int stepsFor4Seconds = 683;
const int stepsFor5Seconds = 853;
const int stepsFor6Seconds = 1024;
const int stepsFor7Seconds = 1195;

Stepper myStepper1(stepsPerRevolution, MOTOR1IN1, MOTOR1IN3, MOTOR1IN2, MOTOR1IN4);
Stepper myStepper2(stepsPerRevolution, MOTOR2IN1, MOTOR2IN3, MOTOR2IN2, MOTOR2IN4);

// === ESP-NOW ===
// CHANGE THIS to your peer's MAC address
uint8_t peerMAC[] = {0x48, 0xE7, 0x29, 0xAD, 0x16, 0x00};
//{0xCC, 0xDB, 0xA7, 0x3F, 0x9C, 0x14}; // BLACK
//RED address is: {0x48, 0xE7, 0x29, 0xAD, 0x16, 0x00}

typedef struct struct_message {
  int forceValue;
} struct_message;

struct_message myData;
struct_message incomingData;

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  myStepper1.setSpeed(5);
  myStepper2.setSpeed(5);

  WiFi.mode(WIFI_STA);
  Serial.println("My MAC: " + WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peerMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(peerMAC)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
    }
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);  // Uses updated function signature
}

// === Motor Behavior ===
void handleMotor1(int value) {
  if (value < 1000) return;
  //else if (value < 600) AwarenessShock1();
  //else if (value < 1000) SoftShock1();
  else if (value < 1500) MediumShock1();
  else if (value < 2000) HardShock1();
  else IntenseShock1();
}

void handleMotor2(int value) {
  if (value < 1000) return;
  //else if (value < 600) AwarenessShock2();
  //else if (value < 1000) SoftShock2();
  else if (value < 1500) MediumShock2();
  else if (value < 2000) HardShock2();
  else IntenseShock2();
}

// === Loop ===
void loop() {
  int localForce = analogRead(FORCE_SENSOR_PIN);
  myData.forceValue = localForce;

  // Send value
  esp_now_send(peerMAC, (uint8_t *)&myData, sizeof(myData));

  // Handle local motor
  Serial.print("Local force: ");
  Serial.println(localForce);
  handleMotor1(localForce);

  delay(500); // Keep it modest to avoid flooding
}

// === ESP-NOW Callbacks ===
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Updated callback with correct signature
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));
  int remoteForce = incomingData.forceValue;
  Serial.print("Remote force: ");
  Serial.println(remoteForce);
  handleMotor2(remoteForce);
}

void TickleShock1() {
  // step for 2 seconds in one direction:
  myStepper1.step(stepsFor2Seconds);
  delay(50);
  // step for 2 seconds in the other direction:
  myStepper1.step(-stepsFor2Seconds);
  delay(100);
 } 

void TickleShock2() {
  // step for 2 seconds in one direction:
  myStepper2.step(stepsFor2Seconds);
  delay(50);
  // step for 2 seconds in the other direction:
  myStepper2.step(-stepsFor2Seconds);
  delay(100);
 } 

void AwarenessShock1() {
  // step for 3 seconds in one direction:
  myStepper1.step(stepsFor3Seconds);
  delay(50);
  // step for 3 seconds in the other direction:
  myStepper1.step(-stepsFor3Seconds);
  delay(100);
}

void AwarenessShock2() {
  // step for 3 seconds in one direction:
  myStepper2.step(stepsFor3Seconds);
  delay(50);
  // step for 3 seconds in the other direction:
  myStepper2.step(-stepsFor3Seconds);
  delay(100);
}

void SoftShock1() {
 // step for 4 seconds in one direction:
  myStepper1.step(stepsFor4Seconds);
  delay(50);
  // step for 4 seconds in the other direction:
  myStepper1.step(-stepsFor4Seconds);
  delay(100);
 }

 void SoftShock2() {
 // step for 4 seconds in one direction:
  myStepper2.step(stepsFor4Seconds);
  delay(50);

  // step for 4 seconds in the other direction:
  myStepper2.step(-stepsFor4Seconds);
  delay(100);
 }

void MediumShock1() {
  // step for 5 seconds in one direction:
  myStepper1.step(stepsFor5Seconds);
  delay(50);

  // step for 5 seconds in the other direction:
  myStepper1.step(-stepsFor5Seconds);
  delay(100);
 }

 void MediumShock2() {
  // step for 5 seconds in one direction:
  myStepper2.step(stepsFor5Seconds);
  delay(50);

  // step for 5 seconds in the other direction:
  myStepper2.step(-stepsFor5Seconds);
  delay(100);
 }


void HardShock1() {
  // step for 6 seconds in one direction:
  myStepper1.step(stepsFor6Seconds);
  delay(50);

  // step for 6 seconds in the other direction:
  myStepper1.step(-stepsFor6Seconds);
  delay(100);
 }

 void HardShock2() {
  // step for 6 seconds in one direction:
  myStepper2.step(stepsFor6Seconds);
  delay(50);
  // step for 6 seconds in the other direction:
  myStepper2.step(-stepsFor6Seconds);
  delay(100);
 }

 void IntenseShock1() {
  // step for 7 seconds in one direction:
  myStepper1.step(stepsFor7Seconds);
  delay(50);
  // step for 7 seconds in the other direction:
  myStepper1.step(-stepsFor7Seconds);
  delay(100);
 }

 void IntenseShock2() {
  // step for 7 seconds in one direction:
  myStepper2.step(stepsFor7Seconds);
  delay(50);
  // step for 7 seconds in the other direction:
  myStepper2.step(-stepsFor7Seconds);
  delay(100);
 }
