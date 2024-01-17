#include <elapsedMillis.h>

#define LED_BUILTIN 2

#define BREATHING_IN_MOTOR 23
#define BREATHING_OUT_MOTOR 22
#define TOUCH_PIN 4

elapsedMillis timeSinceLastBreathCycleStart;
int breathingBPM = 17;
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
  updateTouch();
  checkSerial();
}
