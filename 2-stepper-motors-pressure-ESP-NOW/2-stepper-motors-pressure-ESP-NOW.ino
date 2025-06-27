#include "esp_task_wdt.h"
#include <esp_now.h>
#include <WiFi.h>
#include <Stepper.h>

// === Force Sensor ===
#define FORCE_SENSOR_PIN 36

// built in led
#define LED_PIN 2

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

//Laura this is where you can control how much it goes backwards
const int backwardsStepsOffset = 16;

Stepper myStepper1(stepsPerRevolution, MOTOR1IN1, MOTOR1IN3, MOTOR1IN2, MOTOR1IN4);
Stepper myStepper2(stepsPerRevolution, MOTOR2IN1, MOTOR2IN3, MOTOR2IN2, MOTOR2IN4);

// === ESP-NOW ===
// CHANGE THIS to your peer's MAC address
// uint8_t peerMAC[] = {0xCC, 0xDB, 0xA7, 0x3F, 0x9C, 0x14}; //if you're sending to BLACK
uint8_t peerMAC[] = {0x48, 0xE7, 0x29, 0xAD, 0x16, 0x00}; //if you're sending to RED
//{0xCC, 0xDB, 0xA7, 0x3F, 0x9C, 0x14}; // BLACK
//RED address is: {0x48, 0xE7, 0x29, 0xAD, 0x16, 0x00}


bool motorIsTurning = false;

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
  // esp_now_register_recv_cb(OnDataRecv);  // Uses updated function signature
  esp_now_register_recv_cb(reinterpret_cast<esp_now_recv_cb_t>(OnDataRecv));

  pinMode(LED_PIN, OUTPUT);
  //test built in led
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}

// === Motor Behavior ===
void handleMotor1(int value) {
  Serial.println("Start handling motor 1 with value " + String(value));

  if (value < 1000) {
    // ignore values under 1000
    Serial.println("Value " + String(value) + " is below 1000 — no shock applied.");
    return;
  }

  // Uncomment and adjust these if needed:
  // if (value < 600) {
  //   Serial.println("Value " + String(value) + " — triggering AwarenessShock1()");
  //   AwarenessShock1();
  // } else if (value < 1000) {
  //   Serial.println("Value " + String(value) + " — triggering SoftShock1()");
  //   SoftShock1();
  // }

  else if (value < 1500) {
    // between 1000 and 1500
    Serial.println("Value " + String(value) + " — triggering MediumShock");
    DoShock(myStepper1, stepsFor5Seconds, "motor1", "MediumShock");
  } else if (value < 2000) {
    // between 1500 & 2000
    Serial.println("Value " + String(value) + " — triggering Hardshock");
    DoShock(myStepper1, stepsFor6Seconds, "motor1", "HardShock");
  } else {
    // anything above 2000
    Serial.println("Value " + String(value) + " — triggering IntenseShock1()");
    DoShock(myStepper1, stepsFor7Seconds, "motor1", "IntenseShock");
  }

  Serial.println("Done handling motor 1");
}


void handleMotor2(int value) {
  Serial.println("Start handling motor 2 with value " + String(value));
  if (value < 1000) {
    // ignore values under 1000
    Serial.println("Value " + String(value) + " is below 1000 — no shock applied.");
    return;
  }

  // Uncomment and adjust these if needed:
  // if (value < 600) {
  //   Serial.println("Value " + String(value) + " — triggering AwarenessShock2()");
  //   AwarenessShock2();
  // } else if (value < 1000) {
  //   Serial.println("Value " + String(value) + " — triggering SoftShock2()");
  //   SoftShock2();
  // }

  else if (value < 1500) {
    // between 1000 and 1500
    Serial.println("Value " + String(value) + " — triggering MediumShock");
    DoShock(myStepper2, stepsFor5Seconds, "motor2", "MediumShock");
  } else if (value < 2000) {
    // between 1500 & 2000
    Serial.println("Value " + String(value) + " — triggering Hardshock");
    DoShock(myStepper2, stepsFor6Seconds, "motor2", "HardShock");
  } else {
    // anything above 2000
    Serial.println("Value " + String(value) + " — triggering IntenseShock1()");
    DoShock(myStepper2, stepsFor7Seconds, "motor2", "IntenseShock");
  }
  Serial.println("Done handling motor 2");
}

// === Loop ===
void loop() {
  int localForce = analogRead(FORCE_SENSOR_PIN);
  myData.forceValue = localForce;

  // Send value
  if (!motorIsTurning) {
    //only send if we are not turning the motor
    esp_now_send(peerMAC, (uint8_t *)&myData, sizeof(myData));

    // Handle local motor
    Serial.println("----");
    Serial.print("Measured local force: ");
    Serial.println(localForce);
    handleMotor1(localForce);
  }



  delay(500);  // Keep it modest to avoid flooding
}

// === ESP-NOW Callbacks ===
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Updated callback with correct signature
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  // Correct memcpy usage: copy raw bytes into the struct
  if (motorIsTurning) {
    //if the motor is already turning, disregard this
    Serial.println("Got ESPNow message while the motor was turning, disregarding");
    return;
  }
  memcpy(&myData, incomingData, sizeof(myData));
  int remoteForce = myData.forceValue;
  Serial.print("Received remote force: ");
  Serial.println(remoteForce);
  handleMotor2(remoteForce);
}

// General shock handler
void DoShock(Stepper &motor, int steps, String motorName, String shockName) {
  motorIsTurning = true;
  digitalWrite(LED_PIN, HIGH);
  // disableWDT();

  Serial.println("Starting " + shockName + " on " + motorName + " with " + String(abs(steps)) + " steps");
  SafeStep(motor, steps);
  delay(50);
  SafeStep(motor, -(steps + backwardsStepsOffset ));  //make it go back one more to make sure it always turns off
  delay(100);
  Serial.println("Finished " + shockName + " on " + motorName);
  motorIsTurning = false;
  // enableWDT();
  digitalWrite(LED_PIN, HIGH);
}


void SafeStep(Stepper &stepper, int totalSteps) {
  const int chunkSize = 50;  // smaller chunk sizes help responsiveness
  int stepsLeft = abs(totalSteps);
  int direction = (totalSteps >= 0) ? 1 : -1;
  Serial.println("Im going to move " + String(totalSteps) + " which might take a while...");
  Serial.println("But no worries as I will be yielding every now and then");

  while (stepsLeft > 0) {
    int stepNow = (stepsLeft > chunkSize) ? chunkSize : stepsLeft;
    stepper.step(stepNow * direction);
    stepsLeft -= stepNow;
    yield();               // Let system breathe here
    esp_task_wdt_reset();  // Explicitly feed watchdog
    delay(1);
  }
}



void disableWDT() {
  esp_task_wdt_delete(NULL);  // Unregister current task
}

void enableWDT() {
  esp_task_wdt_add(NULL);  // Re-register current task
}
