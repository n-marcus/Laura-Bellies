
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
  // myStepper2.step(stepsFor7Seconds);
  SafeStep(myStepper2, stepsFor7Seconds);
  delay(50);
  // step for 7 seconds in the other direction:
  // myStepper2.step(-stepsFor7Seconds);
  SafeStep(myStepper2, stepsFor7Seconds);
  delay(100);
}