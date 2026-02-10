#define TOUCH_PIN 4
#define OTHERPOD_BREATHING_IN_MOTOR 19
#define OTHERPOD_BREATHING_OUT_MOTOR 21


bool otherPodIsTouched = false;
bool otherPodBreathingDir = 0;         //1 is breathe in, 0 is neutral, -1 is breathe out
int otherPodBreathInLevel = 0;         //how far have we approximately breathed in the other pod touched lung
int maxOtherPodBreathInLevel = 60000;  //how far can we breathe in before breathing out again, 10000 for big sculpture, 4000 for small one

elapsedMillis printTimerOut;
elapsedMillis printTimerMax;
elapsedMillis printTimerIn;

LPFilter touchValueFilter;


void setupTouch() {
  pinMode(OTHERPOD_BREATHING_OUT_MOTOR, OUTPUT);
  pinMode(OTHERPOD_BREATHING_IN_MOTOR, OUTPUT);
}

void setOtherPodTouched(bool receivedOtherPodIsTouched) {
  //check if the new value is actually different s
  // if (receivedOtherPodIsTouched != otherPodIsTouched) {
  Serial.println("Other pod touched changed to " + String(receivedOtherPodIsTouched));
  otherPodIsTouched = receivedOtherPodIsTouched;
  // }
}

void updateTouchMotors() {

  if (otherPodIsTouched) {
    // Serial.println("Other pod is touched");

    if (otherPodBreathInLevel < maxOtherPodBreathInLevel) {
      otherPodBreathInLevel += 1;
      analogWrite(OTHERPOD_BREATHING_IN_MOTOR, 255);
      analogWrite(OTHERPOD_BREATHING_OUT_MOTOR, 0);
      if (printTimerIn > 1000) {
        Serial.println("Breathing in");
        printTimerIn = 0;
      }

    } else if (otherPodBreathInLevel >= maxOtherPodBreathInLevel) {
      analogWrite(OTHERPOD_BREATHING_IN_MOTOR, 0);
      analogWrite(OTHERPOD_BREATHING_OUT_MOTOR, 0);
      if (printTimerMax > 1000) { 
        Serial.println("Reached max");
        printTimerMax = 0;
      }
    }
  } else if (otherPodIsTouched == 0) {
    // Serial.println("Other pod is not touched");
    if (otherPodBreathInLevel > 0) {
      analogWrite(OTHERPOD_BREATHING_OUT_MOTOR, 255);
      analogWrite(OTHERPOD_BREATHING_IN_MOTOR, 0);
      if (printTimerOut > 1000) {
        Serial.println("Breathing out");
        printTimerOut = 0;
      }
      //we decrease by 2 because if we decrease by 1 it is deflating for too long
      otherPodBreathInLevel -= 2;
    } else {
      // Serial.println("Reached min");
      analogWrite(OTHERPOD_BREATHING_IN_MOTOR, 0);
      analogWrite(OTHERPOD_BREATHING_OUT_MOTOR, 0);
      // otherPodBreathingDir = 0;
    }
  }

  // delay(2);

  //update the otherpod breathing level
  // otherPodBreathInLevel = otherPodBreathInLevel + otherPodBreathingDir;

  // Serial.println("min:0,otherPodTouched:" + String(otherPodIsTouched) + ",otherPodBreathingDir:" + String(otherPodBreathingDir) + ",max:" + String(maxOtherPodBreathInLevel) + ",otherPodBreathingLevel:" + String(otherPodBreathInLevel));
}