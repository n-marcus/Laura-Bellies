void checkSerial() {
  static char buffer[4];  // Buffer to store the received characters
  static byte index = 0;  // Index to keep track of the buffer position

  while (Serial.available() > 0) {
    char incomingChar = Serial.read();

    // Check if the received character is a digit or a '-' sign (for negative numbers)
    if (isdigit(incomingChar) || (incomingChar == '-' && index == 0)) {
      buffer[index++] = incomingChar;

      // Check if the buffer is full
      if (index >= sizeof(buffer) - 1) {
        buffer[index] = '\0';              // Null-terminate the string
        int receivedValue = atoi(buffer);  // Convert the string to an integer

        // Check if the received value is within the valid range (0 to 200)
        if (receivedValue >= 0 && receivedValue < 50) {
          // Perform action based on the received value
          // For example, you can control an LED brightness
          Serial.println("Got " + String(receivedValue) + " from serial, saving as heartbeatrate");
          breathingBPM = receivedValue;
          msPerBreathCycle = 60000 / breathingBPM;
          // analogWrite(LED_BUILTIN, map(receivedValue, 0, 200, 0, 255));
        } else {
          // Invalid value received, handle it as needed
          Serial.println("Invalid value received. Please send a number between 0 and 200, got " + String(receivedValue));
        }

        // Reset the buffer and index for the next input
        index = 0;
      }
    }
  }
}


void printDebug() {

  //print some debugging info
  if (cycleCount % 2000 == 0) {
    Serial.println("min:0.0,max:1.0,");
    Serial.print("Breath_cycle_percentage:" + String(breathCyclePercentage) + ",");
    // Serial.println("Sine:" + String(breathCycleSineWave));
    Serial.print("Breathing_in_value:" + String(breathingInMotorValue / 255.) + ",");
    Serial.print("Breathing_out_value:" + String(breathingOutMotorValue / 255.) + ",");
    // Serial.print("Touch_threshold:" + String(touchBaseLevel - touchThreshold) + ",");
    // Serial.println("Touch_value:" + String(touchValue));
    Serial.println("SwitchState:" + String(switchState));
  }
}