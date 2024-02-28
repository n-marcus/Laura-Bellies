// Insert the MAC address of the receiver board: B
uint8_t broadcastAddress[] = { 0x48, 0xE7, 0x29, 0xAD, 0x16, 0x00 };

esp_now_peer_info_t peerInfo;

void setupESPNow() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(reinterpret_cast<esp_now_recv_cb_t>(OnDataRecv));
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}



// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  bool targetIsMe = receivedData.target == POD_IDENTIFIER;
  Serial.print("---Received data for " + String(receivedData.target));
  if (targetIsMe) Serial.print(" which is me!");
  Serial.println("");

  if (receivedData.breathingsPerMinute == 0 && receivedData.humanPresence == 0 && receivedData.heartbeatRate == 0) {
    Serial.println("All readings are 0");
    Serial.println(" ");
  } else {
    //if everything was 0
    Serial.print("breathingsPerMinute: " + String(receivedData.breathingsPerMinute));
    Serial.print(" Human presence: " + String(receivedData.humanPresence));
    Serial.println(" Heartrate: " + String(receivedData.heartbeatRate));
    Serial.println(" ");
  }

  if (targetIsMe) {
    //save the received human presence
    humanPresence = receivedData.humanPresence;

    //if there is human presence, we should be breathing
    if (humanPresence == 1) {
      shouldBeBreathing = true;
    }


    //if there is a valid breath bpm detected, reflect in the bpm
    if (receivedData.breathingsPerMinute > 0) {
      //it will crash because of a divide by zero error if it receives zero for the bpm
      breathingBPM = receivedData.breathingsPerMinute;
      msPerBreathCycle = 60000 / breathingBPM;
    } else {
      humanPresence = false;
    }

    if (receivedData.breathingsPerMinute == 0 || receivedData.humanPresence == false) {
      //if there is no human detected, return to a verrry slow bpm
      breathingBPM = defaultBreathingBPMWithoutHumanDetected;
      msPerBreathCycle = 60000 / breathingBPM;
    }
  } else {
    Serial.println("Got message for different pod, namely: " + String(receivedData.target));
  }
}