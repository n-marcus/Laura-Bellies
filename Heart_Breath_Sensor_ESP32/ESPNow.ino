long messagesSend = 0;

void setupESPNow() { 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
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
}

void sendESPNowMessage() {

  // Set values to send
  // strcpy(messageToSend.a, "Hellow");

  // if (messageToSend.heartbeatRate > 0 && messageToSend.heartbeatRate > 0) { 
  //   messageToSend.humanPresence = true;
  // } else { 
  //   messageToSend.humanPresence = false;
  // }
  
  // Serial.println(messageToSend.heartbeatRate);
  // Print the struct_message
  printStructMessage(messageToSend);
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&messageToSend, sizeof(messageToSend));

  if (result == ESP_OK) {
    messagesSend ++;
    Serial.println("ESP Now message sent with success " + String(messagesSend));
  } else {
    Serial.println("Error sending the data");
  }
}

// Callback function that will be called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // if (status == ESP_NOW_SEND_SUCCESS) {
  //   Serial.println("Message sent successfully");
  // } else {
  //   Serial.println("Error sending the message");
  // }
}