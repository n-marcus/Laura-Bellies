void setupBreathing() { 
  pinMode(BLOWING_MOTOR_PIN, OUTPUT);
  pinMode(SUCKING_MOTOR_PIN, OUTPUT);
    // Ensure pumps are initially turned off
  analogWrite(BLOWING_MOTOR_PIN, 0);
  analogWrite(SUCKING_MOTOR_PIN, 0);
}

void updateBreathing() {

  //calculate how far we are in the entire breathcycle
  breathCyclePercentage = float(timeSinceLastBreathCycleStart) / float(msPerBreathCycle);

  //convert the cycle percentage into a all-positive sine wave for smooth breathing
  breathCycleSineWave = abs(sin(breathCyclePercentage * TWO_PI));

  //give it a boost by getting the x-th root
  breathCycleSineWave = pow(breathCycleSineWave, 0.3);

  //check if we are breathing in or out based on which half of the breathCycle we are in
  if (breathCyclePercentage < 0.5) {
    //first half of the cycle
    // Serial.println("Breathing in " + String(timeSinceLastBreathCycleStart));
    breathingInMotorValue = breathCycleSineWave * 255.;
    breathingOutMotorValue = 0;
    breathingIn = true;
    analogWrite(LED_BUILTIN, breathingInMotorValue);
  } else {
    //second half of the cycle
    // Serial.println("Breathing out " + String(timeSinceLastBreathCycleStart));
    breathingInMotorValue = 0;
    breathingOutMotorValue = breathCycleSineWave * 255.;
    breathingIn = false;
    digitalWrite(LED_BUILTIN, LOW);
  }


  analogWrite(SUCKING_MOTOR_PIN, breathingInMotorValue);
  analogWrite(BLOWING_MOTOR_PIN, breathingOutMotorValue);

  //print some debugging info
  if (cycleCount % 2000 == 0) {
    Serial.println("min:0.0,max:1.0,");
    Serial.print("Breath_cycle_percentage:" + String(breathCyclePercentage) + ",");
    // Serial.println("Sine:" + String(breathCycleSineWave));
    Serial.print("Breathing_in_value:" + String(breathingInMotorValue / 255.) + ",");
    Serial.println("Breathing_out_value:" + String(breathingOutMotorValue / 255.));
  }

  if (timeSinceLastBreathCycleStart > msPerBreathCycle) {
    //reset the breathing cycle if it is over s
    timeSinceLastBreathCycleStart = 0;
  }
}