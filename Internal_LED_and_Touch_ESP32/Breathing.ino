// #define BREATHING_IN_MOTOR 23
// #define BREATHING_OUT_MOTOR 22

// bool shouldBeBreathing = false;


// void setupBreathing() {
//   pinMode(BREATHING_OUT_MOTOR, OUTPUT);
//   pinMode(BREATHING_IN_MOTOR, OUTPUT);
//   // Ensure pumps are initially turned off
//   analogWrite(BREATHING_IN_MOTOR, 0);
//   analogWrite(BREATHING_OUT_MOTOR, 0);
// }

// void updateBreathing() {

//   if (switchState == 0 && shouldBeBreathing) {
//     //calculate how far we are in the entire breathcycle
//     breathCyclePercentage = float(timeSinceLastBreathCycleStart) / float(msPerBreathCycle);

//     //convert the cycle percentage into a all-positive sine wave for smooth breathing
//     breathCycleSineWave = abs(sin(breathCyclePercentage * TWO_PI));

//     //give it a boost by getting the x-th root
//     breathCycleSineWave = pow(breathCycleSineWave, 0.1);

//     //check if we are breathing in or out based on which half of the breathCycle we are in
//     if (breathCyclePercentage < 0.5) {
//       //first half of the cycle
//       // Serial.println("Breathing in " + String(timeSinceLastBreathCycleStart));
//       breathingInMotorValue = breathCycleSineWave * 255.;
//       breathingOutMotorValue = 0;
//       breathingIn = true;
//       analogWrite(LED_BUILTIN, breathingInMotorValue);
//     } else {
//       //second half of the cycle
//       // Serial.println("Breathing out " + String(timeSinceLastBreathCycleStart));
//       breathingInMotorValue = 0;
//       breathingOutMotorValue = breathCycleSineWave * 255.;
//       breathingIn = false;
//       digitalWrite(LED_BUILTIN, LOW);
//     }



//     if (timeSinceLastBreathCycleStart > msPerBreathCycle) {
//       //reset the breathing cycle if it is over s
//       timeSinceLastBreathCycleStart = 0;
//       numBreathingCycles ++;
//       Serial.println("New breathing cycle: " + String(numBreathingCycles));
//       Serial.println("Breathing BPM is now: " + String(breathingBPM));
//     }
//   }

//   if (breathingIn != _breathingIn) { 
//     Serial.println("Breathing direction changed to: " + String(breathingIn));
//     if (humanPresence == 0) { 
//       //if there is no human presence anymore 
//       shouldBeBreathing = false;
//       breathingInMotorValue = 0;
//       breathingOutMotorValue = 0;
//     }
//   }

//   //keep track of direction changes over time
//   _breathingIn = breathingIn;
//   analogWrite(BREATHING_OUT_MOTOR, breathingOutMotorValue);
//   analogWrite(BREATHING_IN_MOTOR, breathingInMotorValue);
// }