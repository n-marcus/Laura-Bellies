#define TOUCH_PIN 4
#define OTHERPOD_BREATHING_IN_MOTOR 19
#define OTHERPOD_BREATHING_OUT_MOTOR 21


bool otherPodIsTouched = false;
bool otherPodBreathingDir = 0;       //1 is breathe in, 0 is neutral, -1 is breathe out
int otherPodBreathInLevel = 0;       //how far have we approximately breathed in the other pod touched lung
int maxOtherPodBreathInLevel = 300;  //how far can we breathe in before breathing out again

LPFilter touchValueFilter;


void setupTouch() {
  pinMode(TOUCH_PIN, INPUT);

  float average = 0;
  int sum = 0;
  const int NUM_READINGS = 1000;
  for (int i = 0; i < NUM_READINGS; i++) {
    //sum all the readings
    sum += touchRead(TOUCH_PIN);
    delay(1);
  }

  //calculate the average
  average = sum / float(NUM_READINGS);

  Serial.println("Touch average seems to be " + String(NUM_READINGS) + " = " + String(average));
  //save the value globally
  touchBaseLevel = average;
  //delay so we can read the serial
  // delay(2000);

  pinMode(OTHERPOD_BREATHING_OUT_MOTOR,OUTPUT);
  pinMode(OTHERPOD_BREATHING_IN_MOTOR,OUTPUT);
}

void updateTouch() {
  // Read the touch sensor value
  touchValue = touchValueFilter.update(float(touchRead(TOUCH_PIN)), 0.75);

  if (touchValue < touchBaseLevel - touchThreshold) {
    //  Serial.println("I am being touched!");
    beingTouched = true;
  } else {
    beingTouched = false;
  }

  //if touch status changed, send out message to the other pod
  if (beingTouched != _beingTouched) {
    Serial.println("Being touched changed!");
    sendData();
  }

  //keep track of change
  _beingTouched = beingTouched;
  // setOtherPodTouched(beingTouched);
  // updateTouchMotors();
}

void setOtherPodTouched(bool receivedOtherPodIsTouched) {
  //check if the new value is actually different s
  if (receivedOtherPodIsTouched != otherPodIsTouched) {
    Serial.println("Other pod touched changed to " + String(otherPodIsTouched));
    otherPodIsTouched = receivedOtherPodIsTouched;
  }
}

void updateTouchMotors() {

  if (otherPodIsTouched) {
    // Serial.println("Other pod is touched");

    if (otherPodBreathInLevel < maxOtherPodBreathInLevel) {
      otherPodBreathInLevel += 1;
      analogWrite(OTHERPOD_BREATHING_IN_MOTOR, 255);
      analogWrite(OTHERPOD_BREATHING_OUT_MOTOR, 0);
      Serial.println("Breathing in");

    } else if (otherPodBreathInLevel >= maxOtherPodBreathInLevel) {
      analogWrite(OTHERPOD_BREATHING_IN_MOTOR, 0);
      analogWrite(OTHERPOD_BREATHING_OUT_MOTOR, 0);
      // Serial.println("Reached max");
    }
  } else if (otherPodIsTouched == 0) {
    // Serial.println("Other pod is not touched");
    if (otherPodBreathInLevel > 0) {
      analogWrite(OTHERPOD_BREATHING_OUT_MOTOR, 255);
      analogWrite(OTHERPOD_BREATHING_IN_MOTOR, 0);
      Serial.println("Breathing out");
      otherPodBreathInLevel -= 1;
    } else {
      // Serial.println("Reached min");
      analogWrite(OTHERPOD_BREATHING_IN_MOTOR, 0);
      analogWrite(OTHERPOD_BREATHING_OUT_MOTOR, 0);
      // otherPodBreathingDir = 0;
    }
  }

  //update the otherpod breathing level
  // otherPodBreathInLevel = otherPodBreathInLevel + otherPodBreathingDir;

  // Serial.println("min:0,otherPodTouched:" + String(otherPodIsTouched) + ",otherPodBreathingDir:" + String(otherPodBreathingDir) + ",max:" + String(maxOtherPodBreathInLevel) + ",otherPodBreathingLevel:" + String(otherPodBreathInLevel));
}