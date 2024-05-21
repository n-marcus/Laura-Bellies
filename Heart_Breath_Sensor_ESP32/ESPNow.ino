long messagesSend = 0;
long messagesReceived = 0;

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

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(reinterpret_cast<esp_now_recv_cb_t>(OnDataRecv));
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

  //this node will never send anything about touch because it does not have a touch sensor
  messageToSend.isAboutTouch = false;
  messageToSend.breathingPercentage = -1;
  printStructMessage(messageToSend);
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&messageToSend, sizeof(messageToSend));

  if (result == ESP_OK) {
    messagesSend++;
    Serial.println("ESP Now message sent with success #" + String(messagesSend) + " to " + String(TARGET_POD));
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

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  messagesReceived++;
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.println("---Received data #" + String(messagesReceived) + " for " + String(receivedData.target));
  // printStructMessage(receivedData);
}