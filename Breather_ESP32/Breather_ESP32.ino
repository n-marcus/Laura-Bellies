#include <elapsedMillis.h>

#define LED_BUILTIN 2

#define BLOWING_MOTOR_PIN 23
#define SUCKING_MOTOR_PIN 22

elapsedMillis timeSinceLastBreathCycleStart;
int breathingBPM = 5;
float breathingInMotorValue = 0;
float breathingOutMotorValue = 0;
float breathCyclePercentage = 0;
float breathCycleSineWave = 0;
bool breathingIn = false;
int msPerBreathCycle = 60000 / breathingBPM;
long cycleCount = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("I am living");

  pinMode(LED_BUILTIN, OUTPUT);
  setupBreathing();
}

void loop() {
  // put your main code here, to run repeatedly:
  cycleCount ++;
  updateBreathing();
}
