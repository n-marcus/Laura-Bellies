#define TOUCH_PIN 4

void setupTouch() {
  pinMode(TOUCH_PIN, INPUT);

  float average = 0;
  int sum = 0;
  const int NUM_READINGS = 1000;
  for (int i = 0; i < NUM_READINGS ; i ++) { 
    //sum all the readings
    sum += touchRead(TOUCH_PIN);
    delay(1);
  }

  //calculate the average 
  average = sum / float(NUM_READINGS );

  Serial.println("Average of " + String(NUM_READINGS) + " = " + String(average));
  //save the value globally
  touchBaseLevel = average;
  //delay so we can read the serial 
  // delay(2000); 
}

void updateTouch() { 
    // Read the touch sensor value
   touchValue = touchRead(TOUCH_PIN);

   if (touchValue < touchBaseLevel - touchThreshold) { 
     Serial.println("I am being touched!");
     beingTouched = true;
   } else { 
     beingTouched = false;
   }
}