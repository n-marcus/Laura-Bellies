
int switchPin1 = 14;
int switchPin2 = 5;
bool switchPin1State = false;
bool switchPin2State = false;


void setupSwitch() {
  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
}

void updateSwitch() {
  //read the value of the pins of the switch
  switchPin1State = digitalRead(switchPin1);
  switchPin2State = digitalRead(switchPin2);


  //decide the state the switch is in
  if (switchPin1State == true && switchPin2State == true) {
    switchState = 0;
  } else if (switchPin1State == true && switchPin2State == false) {
    switchState = 1;
  } else if (switchPin1State == false && switchPin2State == true) {
    switchState = -1;
  }

  if (switchState != _switchState) {
    //if the state of the button changed
    Serial.println("Switch state changed!");

    //decide how we should be breathing
    if (switchState == 0) {
      breathingOutMotorValue = 0;
      breathingInMotorValue = 0;
    } else if (switchState == 1) {
      breathingOutMotorValue = 255;
      breathingInMotorValue = 0;
    } else if (switchState == -1) {
      breathingOutMotorValue = 0;
      breathingInMotorValue = 255;
    }
    //reset the time since last breath to make sure the breath cycle starts over again
    timeSinceLastBreathCycleStart = 0;
  }

  //keep track of changes
  _switchState = switchState;
}