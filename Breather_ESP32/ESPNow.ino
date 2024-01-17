// Insert the MAC address of the receiver board: B
uint8_t broadcastAddress[] = {0x48, 0xE7, 0x29, 0xAD, 0x16, 0x00};

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
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
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
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  // Serial.print("Char: ");
  // Serial.println(myDataB.a);
  // Serial.print("Is button B pressed = ");
  // Serial.println(myDataB.isButtonBPressed);
  // Serial.print("Touch B Value = ");
  // Serial.println(myDataB.touchValueB); // Print received touchValue
  // Serial.println();

}