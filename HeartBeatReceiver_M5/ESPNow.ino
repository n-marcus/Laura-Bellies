// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Char: ");
  // // Serial.println(myData.a);
  // Serial.println("Got message for pod " + String(myData.target));

  if (myData.target == POD_IDENTIFIER) {
    //if this is for this pod, save the data locally 
    Serial.println("");
    Serial.print("Message is for me: Heartbeat rate = ");
    Serial.print(myData.heartbeatRate);                     // Print received heartbeat
    Serial.print(" with target " + String(myData.target));  // Print received heartbeat
    Serial.println();

    if (myData.heartbeatRate > 0 && myData.heartbeatRate < 300) {
      //if we received a valid heartrate, save it locally
      heartbeatRate = myData.heartbeatRate;
    }

    humanPresence = myData.humanPresence;
  } else { 
    // Serial.println("Message for different pod, skipping!");
    Serial.print(".");
  }
}

void setupESPNow() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  showMacAdresses();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}